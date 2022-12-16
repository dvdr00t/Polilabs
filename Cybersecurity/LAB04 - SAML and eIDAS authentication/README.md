# LAB04 - SAML and eIDAS

1. [Introduction](#introduction)
2. [SAML: logging to the Politecnico website](#saml-logging-to-the-politecnico-website)
3. [SPID: analysis of a SAML authentication](#spid-analysis-of-a-saml-authentication)
4. [eIDAS: analysis of a SAML authentication](#eidas-analysis-of-a-saml-authentication)
    1. [EU Login](#eu-login)
    2. [eIDAS Metadata](#eidas-metadata)
5. [OpenID Connect](#openid-connect)
    1. [Analysis of an authentication login](#analysis-of-an-authentication-login)

## Introduction
In this laboratory, we will perform exercises aimed to experiment with **SAML** and **eIDAS**.

## SAML: logging to the Politecnico website
To experiment with **SAML** messages, we will start with the Login service on the **Politecnico di Torino** website, which actually uses a SAML-based solution to authenticate students and academic staff to provide them access to the services.

By clicking on the `login` button on `polito` website, a `GET` request is performed to the `intranet` endpoint:
```http
GET /intranet/ HTTP/1.1
Host: www.polito.it
Cookie: polito_consenso_cookie=enabled; 
Sec-Ch-Ua: "Not?A_Brand";v="8", "Chromium";v="108"
Sec-Ch-Ua-Mobile: ?0
Sec-Ch-Ua-Platform: "macOS"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.5359.72 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Referer: https://www.polito.it/
Accept-Encoding: gzip, deflate
Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
Connection: close
```

However, the response from the server is a `302 Found` response that **redirects** the client to the **SAML2 Single Sign-On** (**SSO**) server. The response contains the URL to reach in order to complete the authentication. The URL contains the following query parameters:
- `SAMLRequest`: the actual SAML request;
- `RelayState`: an opaque identifier that is passed back without any modification or inspection;
- `SigAlg`: it specifies the algorithm used to sign the request;
- `Signature`: the **base64url-encoded** signature of the request string before this parameter.

After a few more requests, the client is finally redirected to the login page provided by the **identity provider** (**IdP**): `/idp/x509mixed-login`.\
Before moving forward, let's analyze the **SAML Request** generated.

By starting the SAML-tracer add-ons (on Firefox), we can see the SAML-generated request: 
```xml
<samlp:AuthnRequest 
    xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol"
    AssertionConsumerServiceURL="https://www.polito.it/Shibboleth.sso/SAML2/POST"
    Destination="https://idp.polito.it/idp/profile/SAML2/Redirect/SSO"
    ID="_9f1bc4475ad24e512a7d1bbf2fa53d8b"
    IssueInstant="2022-12-12T17:13:16Z"
    ProtocolBinding="urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST"
    Version="2.0">
    
    <saml:Issuer xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion">https://www.polito.it/shibboleth-sp</saml:Issuer>
    <samlp:NameIDPolicy AllowCreate="1"/>

</samlp:AuthnRequest>

```

In particular, we can see the role of the entity which generated the SAML Request, which is: `<saml:Issuer xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion">https://www.polito.it/shibboleth-sp</saml:Issuer>`

[Shibboleth](https://en.wikipedia.org/wiki/Shibboleth_(software)) is a **Single Sign-On** (**SSO**) log-in system for computer networks. It allows people to sign in using just one identity to various systems run by federations of different organizations or institutions. The federations are often universities or public service organizations.

At this point, the browser is able to download the `/idp/Authn/X509Mixed/UserPasswordLogin` webpage where the `username` and `password` form is ready to accept the data from the user.

Once the user has inserted the credential, he is redirected to the Shibboleth endpoint (`/idp/profile/SAML2/Redirect/SSO `) and we can now see the **SAML Response**: 
```xml
<saml2p:Response Destination="https://www.polito.it/Shibboleth.sso/SAML2/POST" ID="_06b466e61ef99d80f11d7c324fa331e5" InResponseTo="_9f1bc4475ad24e512a7d1bbf2fa53d8b" IssueInstant="2022-12-12T17:13:20.476Z" Version="2.0" xmlns:saml2p="urn:oasis:names:tc:SAML:2.0:protocol" xmlns:xs="http://www.w3.org/2001/XMLSchema">

    <saml2:Issuer Format="urn:oasis:names:tc:SAML:2 0:nameid-format:entity" xmlns:saml2="urn:oasis:names:tc:SAML:2.0:assertion">https://idp.polito.it/idp/shibboleth</saml2:Issuer>
    
    <ds:Signature xmlns:ds="http://www.w3.org/2000/09/xmldsig#">
    ...

    ...
/>
```

Note how the response is linked to the request via the `InResponseTo`:
```
InResponseTo="_9f1bc4475ad24e512a7d1bbf2fa53d8b"
```
and it is signed by the IdP with a X.509 certificate:
```xml
<ds:X509Data>
```
Moreover, the response is sent to the URL indicated in the `Destination` field in the request:
```xml
Destination="https://idp.polito.it/idp/profile/SAML2/Redirect/SSO"
```
and again, it is issued by Shibboleth.



## SPID: analysis of a SAML authentication

The **Sistema Pubblico di Identità Digitale** (**SPID**) addresses **e-ID interoperability** at the national level. It is composed by a set of trusted private and public services that can handle authentication of Italian citizens and companies for the public administration. SPID credentials are required to access public services, simplifying the interaction between entities and increasing security of the user authentication. Various credentials can be used, ranging from traditional ones based on smart-cards (e.g. the citizens’ service card, in short CNS) to modern systems (e.g. one-time password generators, implemented as smartphone applications or via a hardware device). SPID components interact with each other using the SAML 2.0 language, for whom a specific profile has been defined.

In the [support](./support/) folder, it is possible to find four SAML packets belonging to a full round of authentication with SPID (captured with SAML tracer).

- The `SAML-B.txt` file contains the **first** SAML Authentication Request which the one analyzed in the previous section. It is a basic redirect from the Politecnico's `intranet` to the **IdP** login page.

- Once the user reaches the login webpage, it can click on the `Login with SPID` button in order to perform the login through SPID. Again, this is a redirect to the `https://identity.sieltecloud.it/simplesaml/saml2/idp/SSO.php` endpoint which contains a SAML Authentication Request to the SPID endpoint. The request is contained in `SAML-D.txt` and it has the following attribute:
```xml
<samlp:RequestedAuthnContext Comparison="minimum">
    <saml:AuthnContextClassRef xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion">https://www.spid.gov.it/SpidL2</saml:AuthnContextClassRef>
</samlp:RequestedAuthnContext>
```

- The `SAML-A.txt` contains the authentication response obtained through the SPID. This is the response to the `SAML-D.txt` request.

- Finally `SAML-C.txt` is the authentication response to the `SAML-B.txt` request and it contains, indeed, the authentication response of the initial login attempt. 

Here some information that can be extracted by the log capture in order to answer questions in the PDF exercise:
- **Protocol version**: it can be found in `SAML-D.txt`, inside the `<samlp:AuthnRequest>` tag. In particular, it is `Version="2.0"`.
- **Protocol binding**: it can be found in `SAML-D.txt`, inside the `<samlp:AuthnRequest>` tag. In particular, it is `ProtocolBinding="urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST"`.
- The **relying party**, again, is Politecnico's Shibboleth.
- The involved **identity providers** are: `identity.sieltecloud.it` and `www.spid.gov.it`.-
- The **validity period** of the SAML responses is: `NotOnOrAfter="2021-11-17T17:15:16.653Z`.
- The **subject** of the `SAML-C.txt` certificate is: `diana.berbecaru@polito.it`


## eIDAS: analysis of a SAML authentication

In this exercise we will experiment with the messages exchanged by the **eIDAS protocol**, which are **SAML- based messages extended to support the attributes defined in eIDAS**, such as the ones for the natural person (`FirstName`, `FamilyName`, `DateOfBirth`, `PersonIdentifier`, ...).

We suppose to start from a real foreign European service, i.e. the **European Commission Authentication Service** (**ECAS**) which is a unified authentication system to access many different EU services. Among many different possible authentication means and Identity Providers, it is possible to choose also the national credentials to authenticate. In our case, we will be redirected to the user Italian IDP for authentication. Since we are using the eIDAS infrastructure, the messages are redirected from the **ECAS** (that act like a Service Provider) to the Italian IdP via two additional elements: the **eIDAS Connector** and the **eIDAS Proxy**.

### EU Login
Let's start by analyzing the authentication performed with eIDAS to the **EU Login** endpoint. EU Login is the European Commission's user authentication service. It allows authorized users to access a wide range of Commission web services, using a single email address and password.

1. First, we connect to the `https://ecas.ec.europa.eu/cas/login` login endpoint. No SAML requested are performed at this step: it is just a web browser performing a `GET` operation to retrieve the webpage `/cas/login`. Once we reach the login website, it is possible to perform the sign in operation with the **eID**. 

2. By clicking on the `Sign in with eID` button, a `GET` and a `POST` operations are performed in order to reach the country dispatcher web page. Again, no SAML messages are exchanged until now. Here, we have the possibility to select the **Member State** that will verify our identity and provide a valid response to the EU website.

3. By selecting `Italy` and clicking on the `Submit` button, a `POST` operation is performed:
```http
POST /cas/selectCountry.cgi HTTP/1.1
Host: ecas.ec.europa.eu
Cookie: ...
...
Accept-Encoding: gzip, deflate
Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
Connection: close

TxId=...&loginRequestId=...&country=IT&proceed=Submit
```

4. Once the country has been submitted, the **first SAML Authentication Request** is sent. Here we can see:
    - the `AuthnRequest` details (such as: `ProviderName`, `Version`, the re-direct `Destination`, etc...);
    ```xml
    <saml2p:AuthnRequest 
        xmlns:ds="http://www.w3.org/2000/09/xmldsig#" 
        xmlns:eidas="http://eidas.europa.eu/saml-extensions"
        xmlns:saml2="urn:oasis:names:tc:SAML:2.0:assertion"
        xmlns:saml2p="urn:oasis:names:tc:SAML:2.0:protocol"
        Consent="urn:oasis:names:tc:SAML:2.0:consent:unspecified"
        Destination="https://service.eid.gov.it/EidasNode/ColleagueRequest"
        ForceAuthn="true"
        ID="..."
        IsPassive="false"
        IssueInstant="2022-12-16T14:10:30.304Z"
        ProviderName="EU Login"
        Version="2.0"
    >
    ```
    - the `Issuer` details; 
    ```xml
    <saml2:Issuer Format="urn:oasis:names:tc:SAML:2.0:nameid-format:entity">https://ecas.ec.europa.eu/cas/eidas/metadata/ecas-ec-europa-eu.xml</saml2:Issuer>
    ```
    - the actual `Signature` of the `ecas-ec-europa-eu` website with the related details;
    - the `eidas:RequestedAttributes`:
    ```xml
    <eidas:RequestedAttributes>
        <eidas:RequestedAttribute 
            FriendlyName="FamilyName"
            Name="http://eidas.europa.eu/attributes/naturalperson/CurrentFamilyName"
            NameFormat="urn:oasis:names:tc:SAML:2.0:attrname-format:uri"
            isRequired="true"/>
        <eidas:RequestedAttribute 
            FriendlyName="FirstName"
            Name="http://eidas.europa.eu/attributes/naturalperson/CurrentGivenName"
            NameFormat="urn:oasis:names:tc:SAML:2.0:attrname-format:uri"
            isRequired="true"/>
        <eidas:RequestedAttribute 
            FriendlyName="DateOfBirth"
            Name="http://eidas.europa.eu/attributes/naturalperson/DateOfBirth"
            NameFormat="urn:oasis:names:tc:SAML:2.0:attrname-format:uri"
            isRequired="true"/>
        <eidas:RequestedAttribute 
            FriendlyName="PersonIdentifier"
            Name="http://eidas.europa.eu/attributes/naturalperson/PersonIdentifier"
            NameFormat="urn:oasis:names:tc:SAML:2.0:attrname-format:uri"
            isRequired="true"/>
    </eidas:RequestedAttributes>
    ```




5. Now, we have been redirected to the `https://service.eid.gov.it/EidasNode/ColleagueRequest` **Italian eIDAS Proxy** server which will act as a proxy to the **Italian IdP** server. 

6. By clicking on `Next` a couple of times, we are redirected to the **Italian IdP** and the **second SAML Authentication Request** is generated. In this case, the request has been generated directly by the Italian Proxy server, and it has the following contents:
    - the `AuthnRequest` details (such as: `ProviderName`, `Version`, the re-direct `Destination`, etc...);
    ```xml
    <saml2p:AuthnRequest 
        xmlns:saml2p="urn:oasis:names:tc:SAML:2.0:protocol"
        xmlns:ds="http://www.w3.org/2000/09/xmldsig#"
        xmlns:eidas="http://eidas.europa.eu/saml-extensions"
        xmlns:saml2="urn:oasis:names:tc:SAML:2.0:assertion"
        Consent="urn:oasis:names:tc:SAML:2.0:consent:unspecified"
        Destination="https://idp-proxy.eid.gov.it/idpproxy/idpeurequest"
        ForceAuthn="true"
        ID="..."
        IsPassive="false"
        IssueInstant="2022-12-16T14:10:47.160Z"
        ProviderName="EU Login"
        Version="2.0"
    >
    ```
    - the `Issuer` details; 
    ```xml
    <saml2:Issuer Format="urn:oasis:names:tc:SAML:2.0:nameid-format:entity">https://service.eid.gov.it/EidasNode/ServiceRequesterMetadata</saml2:Issuer>
    ```
    - the actual `Signature` of the Italian proxy server with the related details;
    - and, of course, the same set of `eidas:RequestedAttributes` from the previous request.

7. At this point, we can perform the authentication with **SPID**. The authentication follows the previously (see section: [SPID: analysis of a SAML authentication](#spid-analysis-of-a-saml-authentication)) analyzed steps in order to produce the **SAML Authentication Response** that will be used to authenticate our identity to the EU Login (through, of course, the Italian Proxy and the EU Connector). Once the operation is completed, we will be redirected again to the **EU Login** endpoint where, at this point, our identity has been confirmed an we can access the required services.

The produced **SAML Authentication Response** contains the following data: 
- the `Response` details (such as: `InResponseTo`, `Version`, the re-direct `Destination`, etc...);
```xml
<saml2p:Response 
    xmlns:ds="http://www.w3.org/2000/09/xmldsig#"
    xmlns:eidas="http://eidas.europa.eu/attributes/naturalperson"
    xmlns:saml2="urn:oasis:names:tc:SAML:2.0:assertion"
    xmlns:saml2p="urn:oasis:names:tc:SAML:2.0:protocol"
    Consent="urn:oasis:names:tc:SAML:2.0:consent:obtained"
    Destination="https://service.eid.gov.it/EidasNode/IdpResponse"
    ID="..."
    InResponseTo="__3crvjvUABSDs3AdTSu.sTVRZwwC.5EU8oLhI-ohDkNsQmkKrNSqjoDcZuXkNYS"
    IssueInstant="2022-12-16T15:06:36.425Z"
    Version="2.0"
>
```
- the `Issuer` details (in this case the Authentication was performed through the **PosteId** italian provider but the Italian Proxy will change the `Issuer` to be itself in order to communicate with the EU Connector): 
```xml
<saml2:Issuer Format="urn:oasis:names:tc:SAML:2.0:nameid-format:entity">https://idp-proxy.eid.gov.it/idpproxy/idpeumetadata</saml2:Issuer>
```
- the actual `Signature` of the Italian proxy server with the related details;
- the `EncryptedAssertion` containing all the details related to the encryption schema used to encrypt the content of the SAML response (along with the X.509 certificate containing the public key, etc...). Here, for example, it is reported the **Algorithm** used to encrypt the content.
```xml
<xenc:EncryptionMethod Algorithm="http://www.w3.org/2009/xmlenc11#aes256-gcm"/>
```

### eIDAS Metadata
To securely interoperate, all the actors in the above workflow (the Service Provider, the eIDAS Connector, the eIDAS Proxy and the user home Identity Provider) must share SAML metadata.

In this section, I will analyze the SAML metadata for the communication between the **EU eIDAS Connector** and the **Italian eIDAS Proxy**.

- The SAML Metadata of the EU eIDAS connector is available [here](https://ecas.ec.europa.eu/cas/eidas/metadata/ecas-ec-europa-eu.xml).
- The SAML metadata of the Italian eIDAS proxy are necessary for the communication with the EU Connector is available [here](https://service.eid.gov.it/EidasNode/ServiceRequesterMetadata).

#### EU eIDAS Connector 
By opening the above given link, the content of the SAML metadata of the EU eIDAS Connector is the following:

- an `EntityDescriptor`, having the `entityID=”https://ecas.ec.europa.eu/ec-europa-eu.xml”`. The `entityID` is like an identifier for the connector. In fact, we can see it in the `<saml2:Issuer>` element of the SAML Request, which is sent from the connector to the proxy. Note also that there is a `validUntil=”..”` which indicates the time limit until the SAML metadata is valid.
```xml
<md:EntityDescriptor entityID="https://ecas.ec.europa.eu/cas/eidas/metadata/ecas-ec-europa-eu.xml" validUntil="2022-12-16T16:54:52.298Z">
```

- the `Signature` that indicates that the SAML Metadata file is digitally signed. Moreover, it indicates also which X.509 certificate needs to be used to verify the signature on the SAML Metadata file. The certificate is found in `<ds:X509Certificate>` element. **Note that the eIDAS Proxy must obtain this certificate through an OOB manner and configure it in a (local) trust store**.

- the `SPSSODescriptor` which contains two elements:
    - `<md:KeyDescriptor use="signing”>`. This element contains an X.509 certificate which must be used by the eIDAS Proxy to verify the signature on the SAML Requests.\
    In fact, if we check (in the SAML-tracer) the SAML Request sent by the Connector to the Proxy, we should note that there is a `<ds:X509Certificate>` element which has the same value with the one indicated in the `<ds:X509Certificate>` in the `<md:KeyDescriptor use="signing">` of the Connector SAML metadata file. Note that the certificate is Base64 encoded.
    ```xml
    <md:KeyDescriptor use="signing">...</md:KeyDescriptor>
    ```
    - `<md:KeyDescriptor use="encryption”>`. This element contains an X.509 certificate which must be used by the eIDAS Proxy to protect the assertion sent in the SAML Response.\
    In fact, if we check in the SAML-tracer the SAML Response sent by the Italian eIDAS Proxy to the EU Connector, we should see an `<saml2:EncryptedAssertion>` element. In this element, the `<ds:X509Certificate>` contains a certificate whose value is equal to the one in the element `<md:KeyDescriptor use="encryption">` of the Connector SAML metadata. Note that the certificate is Base64 encoded.
    ```xml
    <md:KeyDescriptor use="encryption">...</md:KeyDescriptor>
    ```

- the `AssertionConsumerService` is an URL that can be initiated at the identity provider server site or the service provider site: 
```xml
<md:AssertionConsumerService Binding="urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST" Location="https://ecas.ec.europa.eu/cas/login" index="0" isDefault="true"/>
```

Here some information used to answer the original questions in the PDF:
- the algorithm accepted to decrypt the content of the SAML response is **AES-256** in **GCM Mode**:
```xml
<md:EncryptionMethod Algorithm="http://www.w3.org/2009/xmlenc11#aes256-gcm"/>
```
- the actual algorithm used by the eIDAS Service for the protection of the eIDAS Response sent to the eIDAS connector is, indeed, **AES-256** in **GCM Mode**:
```xml
<xenc:EncryptionMethod Algorithm="http://www.w3.org/2009/xmlenc11#aes256-gcm" />
```


#### Italian eIDAS Proxy
By opening the above given link, the content of the SAML metadata of the Italian eIDAS Proxy is the following:

- an `EntityDescriptor`, having the `entityID="https://service.eid.gov.it/EidasNode/ServiceRequesterMetadata"”`. The `entityID` is like an identifier for the proxy. Note also that there is a `validUntil=”..”` which indicates the time limit until the SAML metadata is valid.
```xml
<md:EntityDescriptor entityID="https://service.eid.gov.it/EidasNode/ServiceRequesterMetadata" validUntil="2025-09-11T16:34:44.698Z">
```
- the `SPSSODescriptor` which contains two elements:
    - `<md:KeyDescriptor use="signing”>`. This element contains an X.509 certificate which must be used by the EU eIDAS Connector to verify the signature on the SAML Response.
    ```xml
    <md:KeyDescriptor use="signing">...</md:KeyDescriptor>
    ```
    - `<md:KeyDescriptor use="encryption”>`. This element contains an X.509 certificate which must be used by the EU eIDAS Connector to decrypt the assertion sent in the SAML Response.
    ```xml
    <md:KeyDescriptor use="encryption">...</md:KeyDescriptor>
    ```


> Note that every time the metadata is fetched from the website, new cryptographic parameters are used!


## OpenID Connect

**OpenID Connect** (**OIDC**) is a protocol built on top of **OAuth2** to provide a complete solution for both **authentication** and **authorization**. It allows OAuth2 clients (usually, a service provider from end-user/resource owner perspective) to check the identity of an end-user entity thanks to an authentication performed on a third component, in order to authorize the OAuth2 consent. A nice security outcome, is that **the OAuth2 client does not need to store or manage any password**. In fact, the user delegates authority for the OAuth2 client application (i.e. application server from end user perspective) to access some protected resource on their behalf. Such a resource might be anything (e.g. Google profile information, Facebook friend list). Delegating access to this resource gives the client application the means of verifying the end-user’s identity without ever seeing his credentials

The OAuth2 standard allows for a number of different flows (i.e. message exchange schemas). The most used, and suggested for its security properties, is the **AuthZ Code Grant**. In this flow the response to the Relaying party is not in the form of the actual OAuth2 ticket (i.e., the credentials in OAuth2 terminology), but in the form of a **code**, not containing the user credentials but allowing the relying party to check if the end-user has been successfully authenticated. In fact, the presence of a OAuth2 value of type `token` or `code` allows to distinguish between the implicit flow (a simpler but more insecure one, which allow the end-user credential to flow among the actors, which should be always avoided) and the preferred Authz Code Grant flow. In the following scenario, the AuthZ code is delivered by an AuthZ Server. The AuthZ server acts like a broker between the client and the resource owner. The server authenticates the resource owner and obtains authorization before redirecting it back to the client with the AuthZ code. As pointed out, the resource owner only authenticates versus the authorization server.

A brief description of this flow follows:
1. The flow starts with the client that builds the URI with the necessary parameters and redirects the user on the AuthZ Server dedicated endpoint. The user logs in and accepts/denies to give authorization to the client. Peculiar to OIDC scenario, a subset of the following OpenID Connect scopes is requested:
    - `openid`: it means that the client is making an OIDC request
    - `profile`: this requests access to the user’s profile information
    - `email`: this requests access to the user’s e-mail address
    - `address`: this requests access to the user’s address information
    - `phone`: this requests access to the user’s phone number

2. If the user accepts, the AuthZ Server redirects the user back to the client with an AuthZ code. OIDC defines optional parameters (e.g. `nonce`, `prompt`) for managing the authentication. The server check the parameters and in case of a valid login request, the user is presented with the usual consent screen. The user MAY authorizes the relying party (OAuth2 client) to authenticate it. In this case, he will be redirected back to the client application via the redirection endpoint, along with the authorization code.

3. The client (without user intervention) requests the access token by sending the token request with the AuthZ code to the AuthZ Server. The server can verify the correct relation of the AuthZ Code and the OIDC authentication request by checking the client identifier. It builds up the id token that is a `JWT` which contains private claims about the authenticated user (e.g. name) and other claims like, for example `iss` that represents the issuer and `sub` that represents a unique identifier of the authenticated user.

4. The AuthZ Server respond with the access token, as the property access token). However, it will contain the additional token known as an ID token (as the property id token). The client MUST decrypt the token by using the specified cryptographic operations contained in the `JOSE` Header, validate the `JWT` ID Token, and retrieve the claims (e.g. the user and the issuer). Finally, the user identity provided by the AuthZ Server is verified. Now the client can use the access token for accessing resources on a dedicated resource server.

### Analysis of an authentication login

In the [support](./support/) folder, it is possible to find the files related to the **OpenID** authentication result.

In particular:
1. the first captured packet is [OIDC-1.http](./support/OIDC-1.http). Here we can see that the user, who had clicked on `Login with Google`, will be redirected to the following endpoint: 
```http
Location: https://accounts.google.com/o/oauth2/v2/auth?response_type=code&client_id=758485046783-75l03cv59at80qnvfjhtppadp3bksgn0.apps.googleusercontent.com&scope=email%20profile%20https://www.googleapis.com/auth/contacts.readonly&state=nORfJ5TjhQjKvPoA2iAwOCjrfly4HdrS5SSPPyr_EtM%3D&redirect_uri=http://localhost:8080/oauth2/callback/google
```

2. the application server will then create the state parameter, take the info from the configuration and redirect the user to the **Google OAuth2** page. The capture of the HTTP header is present in the [OIDC-2.http](./support/OIDC-2.http) file:
```http
https://accounts.google.com/o/oauth2/v2/auth
```

3. the user logs exploiting Google as AuthZ server and accepts or denies the AuthZs scopes. After the user acceptance, Google redirects it back on the app server with the AuthZ Code.

4. the app server checks that the state is the same and prepares a `HTTP GET` to the Google Token endpoint [OIDC-3.http](./support/OIDC-3.http).

5. Google checks code and secrets, and gives a `JWT` access token to the app server.

6. the app server sends an `HTTP GET` to the Google endpoint by inserting the access token in the authorization header. The Google API server checks the token and prepares the response as JSON, ending the OAuth2 exchange [OIDC-4.http](./support/OIDC-4.http).

7. finally, the user app displays the information of the logged user.

**Questions**
- Analyzing the [OIDC-1.http](./support/OIDC-1.http) message, identify the authorization server: `https://accounts.google.com/o/oauth2/v2/auth`
- Analyzing the [OIDC-2.http](./support/OIDC-2.http) message, identify the type of OAuth2 adopted flow: `response_type=code` (as previously said, the presence of the `code` identifier, defines the type of OAuth2 used to be `AuthZ Code Grant`)
