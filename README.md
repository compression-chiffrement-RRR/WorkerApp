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
./tests/bin/workerapp-test
```
* To launch the app in release/debug mode
```
./debug/workerapp
```
or
```
./release/workerapp
```


