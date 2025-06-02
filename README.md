# Запуск приложения:
```
export BOOST_VERSION=1.83.0
```
```
docker build -f boost.Dockerfile --build-arg BOOST_VERSION=${BOOST_VERSION} --tag=boost:${BOOST_VERSION} .
```
```
docker run -it --rm -p 8080:8080 -v $(pwd):/mnt boost:${BOOST_VERSION}
```
