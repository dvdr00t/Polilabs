# LAB 02 - X.509 certificates and PKI

1. [Introduction](#introduction)
2. [Analysis and status checking of X.509 certificates](#analysis-and-status-checking-of-x509-certificates)
    1. [Getting and analysing an X.509 certificate](#getting-and-analyzing-an-x509-certificate)
    2. [Certificate status checking](#certificate-status-checking)
        1. [CRL verification](#crl-verification)
        2. [OCSP verification](#ocsp-verification)
        3. [OCSP Stapling](#ocsp-stapling)
    3. [Extended Validation (EV), Organization Validation (OV), Domain Validated (DV)](#extended-validation-ev-organization-validation-ov-domain-validated-dv)
3. [Certificate Transparency](#certificate-transparency)
    1. [Analysing SCT extensions in an X.509 certificate](#analyzing-sct-extensions-in-an-x509-certificate)
    2. [Checking certificate presence in CT logs](#checking-certificate-presence-in-ct-logs)
4. [Certificate chains and PKI models](#certificate-chains-and-pki-models)
    1. [Viewing and verification of simple certificate chains](#viewing-and-verification-of-simple-certificate-chains)
    2. [Viewing of a real Federal PKI](#viewing-of-a-real-federal-pki)

## Introduction
In this laboratory, we will perform exercises to experiment more in depth with **PKIs** and **X.509 certificates**.

Most of the proposed exercises basically use the OpenSSL command line program, which allows the use of several cryptographic functions by means of the OpenSSL shell that can be started with the following command:
```shell
openssl command [ command opts ] [ command args ]
```

In particular, some useful commands are:
- `openssl x509`: used to sign and view an X.509 certificate. Actually, the x509 command is a multi purpose certificate utility: it can be used to display certificate information, convert certificates to various forms, sign certificate requests (behaving thus as a “mini CA”), or edit certificate trust settings.
- `openssl verify`: this command allows the verification of a certificate and its certification chain.
- `openssl crl`: the `crl` command processes CRL files in DER or PEM format. Among the other, it allows for CRL validity verification and conversion from DER to PEM format and vice-versa.
- `openssl ocsp`: the `ocsp` command performs many common OCSP tasks. It can be used to print out requests and responses, create requests and send queries to an OCSP responder, and behave like a mini OCSP server itself.

## Analysis and status checking of X.509 certificates
Let's get to work.
### Getting and analyzing an X.509 certificate
First step is to download the **Politecnico di Torino**'s certificates. This can be done by opening `firefox` and directly downloading the certificate (accessible through the padlock near the URL). The certificate can be seen with the command:
```shell
openssl x509 -in www-polito-it.pem -text -noout
```
and its content is:
- `x509.version_number`: version of x509 format.
- `x509.serial_number`: unique serial number issued by the certificate authority. For consistency, if this value is alphanumeric, it should be formatted without colons and uppercase characters.
- `x509.signature_algorithm`: identifier for certificate signature algorithm.
- `x509.issuer`: the authority which signs the current certificate.
- `x509.validity.not_before`: time at which the certificate is first considered valid.
- `x509.validity.not_after`: time at which the certificate is no longer considered valid.
- `x509.subject_public_key_info`: contains the information about the PK of the subject.
- `x509.X509v3_extensions`: contains the list of extensions for this certificate:
    - `X509v3 Authority Key Identifier`: identifies the CA that signed the Certificate.
    - `X509v3 Subject Key Identifier:`: identifies the subject that is in possession of the certificate.
    - `X509v3 Key Usage`: a multi-valued extension consisting of a list of names of the permitted key usages. The defined values are: 
        - `digitalSignature` 
        - `nonRepudiation`
        - `keyEncipherment`
        - `dataEncipherment`
        - `keyAgreement` 
        - `keyCertSign`
        - `cRLSign`
        - `encipherOnly`
        - `decipherOnly`
    - `X509v3 Extended Key Usage`: a list of values indicating purposes for which the certificate public key can be used.
    - `X509v3 Basic Constraints`: a multi-valued extension which indicates whether a certificate is a CA certificate. The first value is CA followed by `TRUE` or `FALSE`. If CA is `TRUE` then an optional `pathlen` name followed by a nonnegative value can be included.
    - `X509v3 Certificate Policies`: a raw extension that supports all of the defined fields of the certificate extension.
    - `X509v3 CRL Distribution Points`: a multi-valued extension whose values can be either a name-value pair using the same form as subject alternative name or a single value specifying the section name containing all the distribution point values.
    - `Authority Information Access`: gives details about how to retrieve information that related to the certificate that the CA makes available.
    - `CT Precertificate SCTs`: received signed timestamps of the pre-certificates sent to the CT. Every timestamp correspond to the signed timestamp of a different CT log server.

    > To know how **Certificate Transparency** works, check its website: https://certificate.transparency.dev/howctworks/
    - `X509v3 Subject Alternative Name`: allows users to specify additional host names for a single SSL certificate. Often used to refer to a **multi-domain SSL certificate**. An SSL certificate with more than one name is associated using the SAN extension.
- `x509.signature_algorithm`: identifier for certificate signature algorithm.
- `x509.signature_value`: the signature.

### Certificate status checking 
In order to verify the state of a certificate, we can use three different methods:
- CRL verification
- OCSP verification
- OCSP Stapling

#### CRL verification
Verifying a certificate against a **Certificate Revocation List** (**CRL**) consists of the following steps:
- obtain the certificate
- obtain the issuer certificate (CA)
- download and verify the CRL (for authentication, integrity and trust)
- check the certificate status with the CRL

In this case, we can download the CRL from the certificate `crlDistributionPoint` with 
```shell
wget http://geant.crl.sectigo.com/GEANTOVRSACA4.crl
```
and its content can be viewed as:
```shell
openssl crl -inform DER -in <filename>.crl -text | head -n <how_many_lines>
```

The **extensions** are: 
- `X509v3 Authority Key Identifier`:  identifies the CA that signed the Certificate
- `X509v3 CRL Number`: counts how many CRLs the CA has released.

##### Questions
- Assume that the first revoked certificate in this list indicates `Revocation Date: Mar 24 18:11:09 2021 GMT`. If we receive on Nov 6 20:40 2022 a document signed with that certificate short time before , should we accept that document as valid? Nota: the document was signed short time before it was received.
> No, we should not accept the certificate as valid. We received a document signed a lot of time after the revocation of the given certificate. No way to accept this as valid

- Assume that another certificate in the CRL indicates `Revocation Date: Oct 21 11:53:12 2022 GMT`. Let’s assume also that the CRL was issued on `Oct 21 12:20:53 2022 GMT`. Let’s assume that we are on Oct 21 12:05:12 2022 and we have just received a document signed (short time before) with the above certificate. Should we accept the document as valid?
> We have not yet received the CRL, so we accept the certificate as valid.

#### OCSP verification
To check the status of the server certificate with OCSP we can use the command:
```shell
openssl ocsp -issuer www-polito-it-CA.pem -cert www-polito-it.pem -url <ocsp_uri> -resp text
```
where `ocsp_uri` can be obtained with:
```shell
openssl x509 -in www-polito-it.pem -ocsp_uri -noout
```

We can perform the check also on the `cloudflare` certificate by performing the previous steps.
- Which steps do you have to perform?
> download the certificate from the website, download the CA certificate, obtain the OCSP url and ask the OCSP server if the certificate is valid.

- What is the status of the certificate (www.cloudflare.com) ?
> Cert status: good

- What type of response you have received back ? (check the `Produced At` field). What is the problem in case of such OCSP responses ?
> The OCSP response has been pre-computed at `Nov 7 09:13:12 2022`, we are sure that it was valid in that time. Moreover, the response contains no **nonce**, leading to possible replay attacks.

#### OCSP Stapling
OCSP stapling is an optional feature that allows a server certificate to be accompanied by an OCSP response that proves its validity. Because the OCSP response is delivered over an already existing connection, the client does not have to fetch it separately. 


OCSP stapling is used only if requested by a client, which submits the `status_request` extension in the TLS handshake request. A server that supports OCSP stapling will respond by including an OCSP response as part of the TLS handshake.

By running the command: 
```shell
echo | openssl s_client -connect www.sony.com:443 -status
```
we can see that we receive the **OCSP response** along with the status `OCSP response: `, indicating that the server does support OCSP stapling. 
As an example, Google does not support OCSP stapling and the response will be like:
`OCSP response: no response sent`.

### Extended Validation (EV), Organization Validation (OV), Domain Validated (DV)
Some CAs issue TLS server certificates with a particular extension named **Extended Validation**. During administrative verification of an EV certificate request, the owner of the website passes a thorough and globally standardized identity verification process (a set of vetting principles and policies ratified by the CA/Browser forum) to prove exclusive rights to use a domain, confirm its legal, operational, and physical existence, and prove the entity has authorized the issuance of the certificate. This verified identity information is included within the certificate.

The verification process for **Organization Validation** (**OV**) and **Domain Validated** (**DV**) certificates is not as comprehensive as for EV. DV certificates only require a proof that the website owner has administrative control over the domain. OV certificates include some identity information about the site operator, but it is not as extensive as for EV.

- Now consider the first certificate (www.nist.org), can you identify how the Domain Validation is performed?
![Domain validation](./Solution/Images/domain-validation.png)


## Certificate Transparency
> To know how **Certificate Transparency** works, check its website: https://certificate.transparency.dev/howctworks/

### Analyzing SCT extensions in an X.509 certificate
Looking at the Politecnico di Torino's certificate, answer the following questions: why the SCTs have been proposed, and in particular the Certificate Transparency occurred? What does it mean “Precertificate”?
> A CA receives a request for a certificate from a domain owner. It checks that the domain owner has the right to request the certificate, and creates a precertificate, which ties the domain to a public key. A **precertificate** contains all the information a certificate does. It also has a poison extension so that user agents won’t accept it. Precertificates help break a deadlock in CT. Before a CA can log a certificate, the certificate needs an SCT (Signed Certificate Timestamp). But for the certificate to get an SCT, it needs to have been submitted to a log.

### Checking certificate presence in CT logs
We can check whether a certain domain’s certificate is present in the active public certificate transparency log. We can find the **list of current usable logs** in the url https://certificate.transparency.dev/logs/

## Certificate chains and PKI models
### Viewing and verification of simple certificate chains
With `firefox`, by opening the certificate details, it is possible to see also the certificate chain. 
- Which fields in the certificate have been exploited to construct the certificate chain you see?
> The `Issuer Name` has been used to construct the **certification chain**, from the certificate itself to the root CA.
- Why the verification of this chain (as you see it in the browser) is successful?
> Because `firefox` trust the root CA (it has already embedded its certificate) and it is able to verify all the chain from top to bottom.

Now, by taking the files provided as support material for this laboratory ([here](./Solution/Chains/)). Running the following command:
```shell
openssl verify -CAfile GlobalSign_Root_CA.pem www-globalsign-com.pem
```
gives the following error: 
```shell
businessCategory = Private Organization, serialNumber = 578611, jurisdictionC = US, jurisdictionST = New Hampshire, C = US, ST = New Hampshire, L = Portsmouth, street = "2 International Drive, Suite 150", O = "GMO GlobalSign, Inc.", CN = www.globalsign.com
error 20 at 0 depth lookup: unable to get local issuer certificate
error www-globalsign-com.pem: verification failed
```
meaning: 
> This issue occurs due to the certificate chain provided while replacing certificate does not have complete chain.

On the other hand, running: 
```shell
openssl verify -verbose -CAfile <(cat GlobalSign_Extended_Validation_CA.pem GlobalSign_CA.pem GlobalSign_Root_CA.pem) www-globalsign-com.pem
```
gives: 
```shell
www-globalsign-com.pem: OK
```
### Viewing of a real Federal PKI
To have an idea about how complex the **Federal PKIs** can become, we suggest you to visit: https://playbooks.idmanagement.gov/fpki/tools/fpkigraph/. At this site, which is an official site of United States Government, you can see the (complex) connections of various CAs across US Government.


Now let’s assume a user exploits an application (e.g. email) that uses a certificate issued by `DoD EMAIL CA 51`, for example a signed e-mail. Then, he send the signed e-mail to another user, which configured as trusted the `Boeing PCA G3`.
Is the validation of the certificate (attached to the signed e-mail) successful? Which is the certificate path?
> Yes, the validation is correct. The certification path is the following:\
> 1. `DoD EMAIL CA 51`
> 2. `DoD Root CA 3`
> 3. `DoD Interoperability Root CA 2`
> 4. `Federal Bridge CA G4`
> 5. `CertiPath Bridge CA - G3`
> 6. `Boeing PCA G3`