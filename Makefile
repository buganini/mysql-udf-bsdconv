PREFIX?=	/usr/local

BSDCONV_FLAGS=	-I${PREFIX}/include -L${PREFIX}/lib -lbsdconv
MYSQL_FLAGS=	-I${PREFIX}/include/mysql
INSTALL_PATH=	${PREFIX}/lib/mysql/plugins

all: udf_bsdconv.so

udf_bsdconv.so: udf_bsdconv.c
	 $(CC) -shared -fPIC ${BSDCONV_FLAGS} ${MYSQL_FLAGS} -o udf_bsdconv.so udf_bsdconv.c

install:
	install -m 555 udf_bsdconv.so ${INSTALL_PATH}

clean:
	rm udf_bsdconv.so
