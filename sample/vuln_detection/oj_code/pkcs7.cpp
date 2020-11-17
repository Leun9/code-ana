#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/pkcs7.h>
#include <openssl/pem.h>
#include <openssl/objects.h>
#include <openssl/x509.h>

const char certificate[] = "\
-----BEGIN CERTIFICATE----- \n\
MIIB/zCCAaagAwIBAgIJAKKa0PAt9M1FMAoGCCqBHM9VAYN1MFsxCzAJBgNVBAYT \n\
AkNOMQ4wDAYDVQQIDAVIdUJlaTEOMAwGA1UEBwwFV3VIYW4xDTALBgNVBAoMBEhV \n\
U1QxDDAKBgNVBAsMA0NTRTEPMA0GA1UEAwwGY2Fyb290MB4XDTIwMDkyMDIwNTkx \n\
OVoXDTMwMDkxODIwNTkxOVowWzELMAkGA1UEBhMCQ04xDjAMBgNVBAgMBUh1QmVp \n\
MQ4wDAYDVQQHDAVXdUhhbjENMAsGA1UECgwESFVTVDEMMAoGA1UECwwDQ1NFMQ8w \n\
DQYDVQQDDAZjYXJvb3QwWTATBgcqhkjOPQIBBggqgRzPVQGCLQNCAASJ8mm28JJR \n\
bZKLr6DCo1+KWimpKEsiTfZM19Zi5ao7Au6YLosyN71256MWmjwkwXxJeLa0lCfm \n\
kF/YWCX6qGQ0o1MwUTAdBgNVHQ4EFgQUAL5hW3RUzqvsiTzIc1gUHeK5uzQwHwYD \n\
VR0jBBgwFoAUAL5hW3RUzqvsiTzIc1gUHeK5uzQwDwYDVR0TAQH/BAUwAwEB/zAK \n\
BggqgRzPVQGDdQNHADBEAiAaZMmvE5zzXHx/TBgdUhjtpRH3Jpd6OZ+SOAfMtKxD \n\
LAIgdKq/v2Jkmn37Y9U8FHYDfFqk5I0qlQOAmuvbVUi3yvM= \n\
-----END CERTIFICATE----- \n\
";

const char Bob_private_key[] = "\
-----BEGIN EC PARAMETERS----- \n\
BggqgRzPVQGCLQ== \n\
-----END EC PARAMETERS----- \n\
-----BEGIN EC PRIVATE KEY----- \n\
MHcCAQEEINQhCKslrI3tKt6cK4Kxkor/LBvM8PSv699Xea7kTXTToAoGCCqBHM9V \n\
AYItoUQDQgAEH7rLLiFASe3SWSsGbxFUtfPY//pXqLvgM6ROyiYhLkPxEulwrTe8 \n\
kv5R8/NA7kSSvcsGIQ9EPWhr6HnCULpklw== \n\
-----END EC PRIVATE KEY----- \n\
";
const int MAX = 10000;
char msg[MAX], res[MAX];
int res_len;


PKCS7* to_PKCS7(const char *txt)
{
	BIO *bio_p7 = BIO_new_mem_buf((char*)txt, strlen(txt));
  	if(bio_p7 == NULL) return NULL;
  	return PEM_read_bio_PKCS7(bio_p7, NULL, NULL, NULL);
}

EVP_PKEY* get_pkey(const char *private_key)
{
	BIO *bio_pkey = BIO_new_mem_buf((char*)private_key, strlen(private_key));
	if(bio_pkey == NULL) return NULL;
 	return PEM_read_bio_PrivateKey(bio_pkey, NULL, NULL, NULL);
}

X509* get_X509(const char *cert)
{
	BIO *bio = BIO_new(BIO_s_mem());
  	if(bio == NULL) return NULL;
	BIO_puts(bio, cert);
  	return PEM_read_bio_X509(bio, NULL, NULL, NULL);
}

bool check() {
    memset(msg, 0, MAX * sizeof(char));
    int i = 0; char ch;
    while((ch = getchar()) != EOF) msg[i++] = ch;
    PKCS7 *p7 = to_PKCS7(msg);
    if(p7 == NULL) return false;
    EVP_PKEY *Bob_pkey = get_pkey(Bob_private_key);
    if(Bob_pkey == NULL) return false;

    BIO *bio_res = PKCS7_dataDecode(p7, Bob_pkey, NULL, NULL);
    if(bio_res == NULL) return false;
    res_len = BIO_read(bio_res, res, MAX);
    if(res_len <= 0) return false;

    STACK_OF(PKCS7_SIGNER_INFO) *signer_stack = PKCS7_get_signer_info(p7); 
    if(signer_stack == NULL) return false;

    X509_STORE *store = X509_STORE_new();
    X509_STORE_CTX *ctx = X509_STORE_CTX_new();
    if(store == NULL || ctx == NULL) return false;
    
    X509_STORE_add_cert(store, get_X509(certificate));
    
    int signer_cnt = sk_PKCS7_SIGNER_INFO_num(signer_stack);
    if(signer_cnt <= 0) return false;  
    
    for(int i = 0; i < signer_cnt; ++i) {  
        PKCS7_SIGNER_INFO *signer_info = sk_PKCS7_SIGNER_INFO_value(signer_stack, i);
        if(signer_info == NULL) return false; 
        if(PKCS7_dataVerify(store, ctx, bio_res, p7, signer_info) != 1) return false;
    }
    
    return true;   
}

int main()
{
  OpenSSL_add_all_algorithms();

	if (check()) {
    for(int i = 0; i < res_len; ++i) putchar(res[i]);
    putchar('\n');
  }
  else printf("ERROR\n");

    return 0;
}