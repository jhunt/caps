CFLAGS  += -static
IMAGE   := huntprod/caps
VERSION := latest

build: caps
all: caps docker
clean:
	rm -f caps.inc caps *.o

caps.inc: caps.lst
	./lst2inc $< >$@
caps.c: caps.inc

docker:
	docker build -t $(IMAGE):$(VERSION) .
push: docker
	docker push $(IMAGE):$(VERSION)

.PHONY: default clean docker push
