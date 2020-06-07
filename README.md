# WorkerApp
Encryption/Compression worker client for Ciphernet app.
### Build and run
* Build the docker image by running this command:
```
docker build -t ciphernet_worker_app <PATH_TO_REPOSITORY>
```
* To build the executables within the container:
```
docker run -it -v <PATH_TO_REPOSITORY>:/root ciphernet_worker_app
make all
```
* To start tests
```
make tests
```
* To launch the app in release/debug mode
```
./debug/workerapp
```
or
```
./release/workerapp
```

### AMQP messages format

The messages to be received by the worker look like this:

```
{ 
	"fileUrl": "https://www.w3.org/TR/PNG/iso_8859-1.txt", 
	"responseUrl": "http://mydomain.com/whereToReceiveResponseData",
	"fileID": "TheFinalFileName",
	"processes": [
		{
			"type": "ENCRYPT_AES_128_ECB",
			"key": [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
		},
		{
			"type": "DECRYPT_AES_128_ECB",
			"key": [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
		}
	] 
}
```

The `fileUrl` property is the file URL to download from the API.

The `responseUrl` property is the API URL to be used for sending a success/error message when message has been processed.

The `fileID` property is the name of the file to be written after processing.

The `processes` is an array of processes objects. A process object has at least one string property which is `type`, it tells the worker what kind of process we should apply.

All processes are executed following the indexes of the `processes` array.
Extra parameters such as `key` or `iv` must be supplied for some processe types.

Here is a list of all the available processes with the required parameters:

##### ENCRYPT_AES_128_ECB

`key` parameter must be an array of 16 bytes

##### ENCRYPT_AES_192_ECB

`key` parameter must be an array of 24 bytes

##### ENCRYPT_AES_256_ECB

`key` parameter must be an array of 32 bytes

##### DECRYPT_AES_128_ECB

`key` parameter must be an array of 16 bytes

##### DECRYPT_AES_192_ECB

`key` parameter must be an array of 24 bytes

##### DECRYPT_AES_256_ECB

`key` parameter must be an array of 32 bytes

##### ENCRYPT_AES_128_CBC

`key` parameter must be an array of 16 bytes

`iv` parameter must be an array of 16 bytes

##### ENCRYPT_AES_192_CBC

`key` parameter must be an array of 24 bytes

`iv` parameter must be an array of 16 bytes

##### ENCRYPT_AES_256_CBC

`key` parameter must be an array of 32 bytes

`iv` parameter must be an array of 16 bytes

##### DECRYPT_AES_128_CBC

`key` parameter must be an array of 16 bytes

`iv` parameter must be an array of 16 bytes

##### DECRYPT_AES_192_CBC

`key` parameter must be an array of 24 bytes

`iv` parameter must be an array of 16 bytes

##### DECRYPT_AES_256_CBC

`key` parameter must be an array of 32 bytes

`iv` parameter must be an array of 16 bytes

##### COMPRESS_LZ78

##### DECOMPRESS_LZ78

### API JSON response

When the file has been processed by the worker, a POST request will be sent to the API's URL supplied in the AMQP message's property `responseUrl`.

It will contain the following JSON on success:

```
{
  "fileID": "TheFinalFileName",
  "success": true
}
```

If something wrong happened:

```
{
  "error": "DERYPT_AES_128_CBC is not a valid process type.",
  "fileID": "TheFinalFileName",
  "success": false
}
```

Note that, in some cases, the worker won't be able to send a response, for example when `responseUrl` is not supplied.


