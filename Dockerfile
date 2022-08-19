FROM gcr.io/tink-containers/debian:bullseye as builder

RUN apt-get update && apt-get install -y make gcc python ca-certificates

COPY . /build
WORKDIR /build
RUN ./ya make -r catboost/libs/model_interface

WORKDIR /build/catboost/libs/model_interface
RUN gcc -o catboost catboost.c -I/build/catboost/libs/model_interface -L/build/catboost/libs/model_interface -lcatboostmodel 
RUN mkdir -p /project/catboost 
RUN cp "$(readlink libcatboostmodel.so.1)" catboost /project/catboost 

FROM gcr.io/distroless/base
COPY --from=builder /project/catboost /
