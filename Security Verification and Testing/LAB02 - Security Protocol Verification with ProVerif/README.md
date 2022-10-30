# LAB 02 - Security Protocol Verification with ProVerif

1. [Introduction](#introduction)
    1. [Structure of a script file](#structure-of-a-script-file)
2. [The Handshake Protocol](#the-handshake-protocol)
    1. [Running the protocol](#running-the-protocol)
    2. [Fixing the protocol](#fixing-the-protocol)
    3. [Verifying the authenticity of the secret](#verifying-the-authenticity-of-the-secret)
    4. [Verifying the integrity of the secret when different secrets can be sent](#verifying-the-integrity-of-the-secret-when-different-secrets-can-be-sent)
3. [The Hash Protocol](#the-hash-protocol)
    1. [Running the protocol](#running-the-protocol-1)
    2. [Fixing the protocol](#fixing-the-protocol-1)
4. [The Signature Protocol](#the-signature-protocol)


## Introduction
The purpose of the laboratory is to assess Security Verifications on three different custom protocols with **ProVerif**. ProVerif is a tool for automatically analyzing the security of cryptographic protocols, capable of proving *reachability properties*, *correspondence assertions*, *observational equivalence* and moreover, capable of *attack reconstruction*: when a property cannot be proved, ProVerif tries to reconstruct an execution trace that falsifies the desired property.

To get familiar with the ProVerif's syntax of operations, take a look at the official manual that can be downloaded [here](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwjEh9nQyIf7AhXxgP0HHZ1SBfkQFnoECA0QAQ&url=https%3A%2F%2Fbblanche.gitlabpages.inria.fr%2Fproverif%2Fmanual.pdf&usg=AOvVaw3XfcJirzmkahow42sZ2LAu). 

Supposedly you have already installed ProVerif, here's some useful `commands`:
- To run ProVerif on a script file, add the `proverif` binary file to the `$PATH` variable and run: 
```bash
proverif <filename>.pv
```
- Run ProVerif on a script file and request ProVerif to generate an attack trace graph in a given directory:
```bash
proverif -graph directory filename
```
- Run ProVerif on a script file in interactive mode to perform a simulation:
```bash
proverif interact filename
```

### Structure of a script file
A ProVerif script file is divided into three main parts: 
- **Declarations**: the declarations formalize the behavior of cryptographic primitives. 
- **Process Macros**: process macros allow sub-processes to be defined, in order to ease development.
- **Processes**: the protocol itself can be encoded as a main process, with the use of macros.

## The Handshake Protocol
The **handshake protocol** is a custom and simple protocol, based on **asymmetric cryptography**, used to securely exchange a secret between a **client process** and a **server process**. It can be found here: [handshake1cie.pv](./Handshake%20Protocol/handshake1cie.pv).

The protocol defines some asymmetric and symmetric functions to encrypt, decrypt, sign and verify a secret message `s`. Four **events** are defined in the script, that will be tested against the queries later:
- `event request_auth(pkey, pkey, bitstring).`
- `event confirm_auth(pkey, pkey, bitstring).`
- `event server_is_reachable()`
- `event client_is_reachable()`

The queries, indeed, are the following: 
- `query attacker(s).` to check whether a potential attacker has access to the secret message `s`.
- `query event(client_is_reachable()).` to check whether, at the end of the protocol, the client has completed its duty and it is ready to communicate. 
- `query event(server_is_reachable()).` to check whether, at the end of the protocol, the server has completed its duty and it is ready to communicate. 
- `query client_publickey: pkey, server_publickey: pkey, key: bitstring; inj-event(confirm_auth(client_publickey, server_publickey, key)) ==> inj-event(request_auth(client_publickey, server_publickey, key)).` which means: if a the **server process** with public key `server_publickey` correctly receives key `key`, apparently coming from a **client process** with public key `client_publickey`, then a **client process** with public key `client_publickey` really sent key `key` to the same process and each receive operation corresponds to a distinct send operation (**injectivity**).

### Running the protocol
By running ProVerif on this script, from the ProVerif verification report, we can see that properties about the confidentiality of the secret `s` and properties about the reachability of the processes are satisfied (note that reachability is satisfied if the queries return `false`), while the last property is not satisfied. Indeed, the report specifies that the **non-injective correspondence holds**, i.e. only injectivity does not hold.
```bash
RESULT not attacker(s[]) is true.
RESULT not event(client_is_reachable) is false.
RESULT not event(server_is_reachable) is false.
RESULT inj-event(confirm_auth(client_publickey_1,server_publickey_1,key)) ==> inj-event(request_auth(client_publickey_1,server_publickey_1,key)) is false.
RESULT (but event(confirm_auth(client_publickey_1,server_publickey_1,key)) ==> event(request_auth(client_publickey_1,server_publickey_1,key)) is true.)
```

From the graphical attack trace, it can be understood that the attacker, having access to the encrypted signed key shared on the channel `c`, can perform a replay attack and confirm the authentication before the client. Indeed, he can place himself between the client and the server, receive the encrypted signed key and send to the server process the same value. This is why the **injective property** does not hold but the **non-injective one** does.

**The trace is the following:**
- The attacker can access the public keys of the client and the server.
![handshake_1](./Handshake%20Protocol/images/handshake_1.png)
- The attacker can access the encrypted signed key sent from the client to the server.
![handshake_2](./Handshake%20Protocol/images/handshake_2.png)
- The attacker can send the encrypted signed key to the server, corrupting the authentication:
![handshake_3](./Handshake%20Protocol/images/handshake_3.png)

### Fixing the protocol
In order not to let the attacker replay the encrypted signed key message, a **challenge-response** mechanism is required. The server must send an encrypted **nonce** (which acts as a `session ID`) to the client, encrypted with his public key. The client will reply with the same nonce along with the key.

The fixed protocol can be found at: [handshake1cie_fixed.pv](./Handshake%20Protocol/handshake1cie_fixed.pv)


### Verifying the authenticity of the secret
In order to verify that the secret sent by the server process to the client process is authentic and that integrity holds for it, i.e., whenever the client process receives the secret, indeed the secret was sent by the server process, and the correspondence is injective, i.e. each receive of the secret is preceded by a distinct send of the secret, we need to **introduce other events in the protocol description**.

In particular:
- A new query injective-events is necessary, that identifies when a specific secret has been received it is actually the secret sent by the server:
```bash
query client_publickey: pkey, server_publickey: pkey, scr: bitstring;
    inj-event(secret_recv(client_publickey, server_publickey, scr)) ==> inj-event(secret_sent(client_publickey, server_publickey, scr)).
```
The result is the following:
```bash
RESULT not attacker(s[]) is true.
RESULT not event(client_is_reachable) is false.
RESULT not event(server_is_reachable) is false.
RESULT inj-event(confirm_auth(client_publickey_1,server_publickey_1,nonce_2,key)) ==> inj-event(request_auth(client_publickey_1,server_publickey_1,nonce_2,key)) is true.
RESULT inj-event(secret_recv(client_publickey_1,server_publickey_1,scr)) ==> inj-event(secret_sent(client_publickey_1,server_publickey_1,scr)) is true.
```
### Verifying the integrity of the secret when different secrets can be sent
By modifying the server process macro to send different secrets:
```bash
let server(server_keymaterial: keymat, client_publickey: pkey, s: bitstring) = 
```
it is possible to verify the indeed every session receives its own secret and not another one.
```bash
RESULT not attacker(s1[]) is true.
RESULT not attacker(s2[]) is true.
RESULT not event(client_is_reachable) is false.
RESULT not event(server_is_reachable) is false.
RESULT inj-event(confirm_auth(client_publickey_2,server_publickey_1,nonce_3,key)) ==> inj-event(request_auth(client_publickey_2,server_publickey_1,nonce_3,key)) is true.
RESULT inj-event(secret_recv(client_publickey_2,server_publickey_1,scr)) ==> inj-event(secret_sent(client_publickey_2,server_publickey_1,scr)) is true.
```


## The Hash Protocol
The **Hash Protocol** is non-secure hash protocol which makes use of a *weaksecret* `s` with low-entropy and a low range of allowed value. The script can be found at: [hash.pv](./Hash%20Protocol/hash.pv)

### Running the protocol
By running ProVerif on this script, from the ProVerif verification report, we can see that an attacker can perform an **offline** attack on the hash obtained. From the graphical view: 
![hash](./Hash%20Protocol/hash.png)
The result is: `Using the function hash the attacker may obtain hash(s[]) (resp. hash(@weaksecretcst)) in off-line phase.`

### Fixing the protocol
In order to fix this protocol, a **high-entropy** secret can be add to the hashed message: 
```bash
let sender() = 
    out(c, hash((s, high_entropy_secret)));
0.
```
In this case, the result is: 
```bash
RESULT Weak secret s is true.
RESULT not attacker(s[]) is true.
```

## The Signature Protocol
Last but not least: the **signature protocol** is a simple protocol used to guarantee the authenticity of a software issued and delivered by a software vendor. The [software_signature.pv](./Signature%20Protocol/software_signature.pv) script tests the security of the protocol. 

The result is the following: 
```bash
RESULT event(verified(sw_3,swd_3)) ==> event(issued(sw_3,swd_3)) is true.
```
