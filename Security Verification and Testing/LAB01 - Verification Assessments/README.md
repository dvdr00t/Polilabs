# LAB 01 - Vulnerability Assessment and Penetration Testing (VAPT)

1. [Introduction](#introduction)
2. [Vulnerability Assessment](#vulnerability-assessment)
3. [Vulnerability Exploitation](#vulnerability-exploitation)

## Introduction
The purpose of the laboratory is to perform **vulnerability assessment** and **vulnerability exploitation** on the *Metasploitabl 2* machine. From the documentation, what is Metasploitable:


> A test environment [used to] provides a secure place to perform penetration testing and security research.

During the laboratory at LABINF, the set-up of the machines was the following:

- The IP range of the **LAN** was: `192.168.1.0/24`.
- A **Kali Linux** machine used as *attacker / penetration tester*. 
    - IP address: `192.168.1.xx`. My computer was `192.168.1.50`.
- A **Metasploitable 2** machine used as vulnerable machine for testing.
    - IP address: `unknown` at the beginning. `192.168.1.3` after *host discovery*.

#### Host discovery

First step to be performed was **host discovery**. I knew that the machine was on the LAN but I did not know at which specific IP address. Using `nmap`, with the right-tuned parameters, I was able to figure out where was the vulnerable machine. The command is the following;

```perl
nmap -sn -sV 192.168.1.9/24
```

The options: 
- `-sn`: disable port scan. 
- `-sV`: probe open ports to determine service/version info.

Basically, I was telling nmap to discover which hosts where active in the subnet with netmask: `255.255.255.0` (note the absence of the option `-Pn`, which, instead, performs port scanning even for non-running hosts). To this active hosts, nmap would determine which services the host was running.

The output of the execution was a semi-long list of hosts (c.a 20/25, as not so much students were present in the LAB) which contains the *Metasploitable 2* machine at address `192.168.1.3`.

The machine had a long list of open ports running services, such as (reporting here only the most important): 
| PORT | SERVICE | STATE | VERSION |
| :--: | :--:    | :--: | :--: |
| `21/tcp` | ftp | open | vsftps 2.3.4 |
| `22/tcp` | ssh | open | OpenSSH 4.7p1 Debian 8ubuntu 1 (protocol 2.0) |
| `513/tcp` | login? | open | r service |
| `1524/tcp` | shell |  open | Metasploitable root shell | 
| `5900/tcp` | vnc |  open | VNC protocol v1.3 | 
| `...` | ... |  ... | ... |
 

## Vulnerability Assessment
Next step: try to find and understand which are the vulnerabilities in the system in order to exploit them.

There are two possible solutions to this problem: one from the attacker perspective and one from the defender perspective.

#### A defender perspective: GVM
**Greenbone Vulnerability Manager** or **GVM** is a vulnerabilities scanning tool which is able to probe a vulnerable system, running scripts and produce a report of all the vulnerabilities found in the system.

Yeah, pretty boring stuff, just tune a little the scanner and then wait A LOT (like an hour) to see a great and cool report of all the stuff GVM had found. 

#### An attacker perspective: nmap
A quicker and dirtier approach (like a real attacker would do) is to use `nmap` itself. Indeed, `nmap` has the possibility to run a list of scripts that performs a GVM-like scan (but lighter and without producing a fancy report) and outputs a list of **CVEs** found on the system.

The command is the following: 
```perl
nmap -sV --scripts vulners 192.168.1.3
```

Where `vulners` is a script that, for each available CPE, prints out known vulnerabilities (links to the correspondent info) and correspondent CVSS scores.

Easy peasy, the result was something like:
```
  PORT        STATE           SERVICE         VERSION
5900/tcp      OPEN              vnc        VNC protocol v1.3    
  |
  | CVE-20xx-xxxx etc..
  | CVE-20xx-xxxx etc..
  | CVE-20xx-xxxx etc..

```


## Vulnerability Exploitation
Now that we know all the vulnerabilities on the running services, we can proceed to the exciting part!

> Spoiler: no excitement.

Last step was to run **Metasploitable**, i.e. a framework already installed in Kali Linux which contains a long database of exploits already written for us to be used against the vulnerable machine. It is a matter of:

- Understanding which exploit needs to be used. This operation can be done by searching the DB for the most suitable exploit for every vulnerability. The command is the following: 
```perl
msf16> search KEYWORD(S)
```

- Load the exploit with the command: 
```perl
msf16> use EXPLOIT_PATH
```

- Tune the exploit by setting specific options to specific values with the command: 
```perl
msf16> set OPTION VALUE
```

- Run the exploit with the command:
```perl
msf16> run
```

- Enjoy your attack!