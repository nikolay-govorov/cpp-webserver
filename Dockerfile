FROM gcc:latest

WORKDIR /app

RUN apt-get update && apt-get install libevent-dev

COPY static /app/static/
COPY src /app/src/
COPY Makefile /app/

RUN make

CMD ["/app/cpp-web-server"]
