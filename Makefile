PREFIX?=	/usr/local
LOCALBASE?=${PREFIX}

BSDCONV_FLAGS=	-I${LOCALBASE}/include -L${LOCALBASE}/lib -lbsdconv
MYSQL_FLAGS=	-I${LOCALBASE}/include/mysql
INSTALL_PATH=	${PREFIX}/lib/mysql/plugins

all: udf_bsdconv.so

udf_bsdconv.so: udf_bsdconv.c
	 $(CC) -shared -fPIC ${BSDCONV_FLAGS} ${MYSQL_FLAGS} -o udf_bsdconv.so udf_bsdconv.c

install:
	install -m 555 udf_bsdconv.so ${INSTALL_PATH}

clean:
	rm udf_bsdconv.so
