BUILD_PATH=./
OUT_LIB_FILE=./lib/*
OUT_BIN_FILE=./bin/*
OUT_MAKE_FILE=./CMakeCache.txt
tmp_INSTALL_DIR=/usr/local/cuprumtest
OPENSSL_DIR=./Source/ThirdParty/openssl-1.0.0d
CPU_NUM=0

if [ -z $1 ];then
	echo "Usage:"
	echo "  ./build.sh [release] [4]"
	echo "  	use 4 processor to build release version"
	echo "  ./build.sh [deb_package]"
	echo "  	 build deb_package"
	echo "  ./build.sh [clean]"
	echo "  	clean out file before build"
fi

#test cup process num
cat /proc/cpuinfo | grep "cpu cores" | while read cpu cores dot cpu_num
do
  	     echo $cpu_num >.tmp_cpu_num
done
cat .tmp_cpu_num && CPU_NUM=`cat .tmp_cpu_num` || CPU_NUM=1
CPU_NUM=$[CPU_NUM+CPU_NUM]

if [ -z $2 ];then
	echo
else
	if [ $2 -lt $CPU_NUM ]
	then
		CPU_NUM=$2
	fi
fi

pushd $BUILD_PATH
case $1 in
"clean" )
	echo "clean out file..."
		rm -rf $OUT_MAKE_FILE && echo "	clean CMakeCache.txt success "
		rm -rf $OUT_LIB_FILE && echo "	clean lib success"
		rm -rf $OUT_BIN_FILE && echo "	clean bin success"
	echo "clean end..."
	;;
"release" )

	#add by luyue
	cd $OPENSSL_DIR
	./config && make && cd -
	mkdir lib 
        cp -rf $OPENSSL_DIR/lib*.a ./lib
	
	echo "build release version start..." && sleep 3
	cmake -DPORT=GTK -DDEVELOPER_MODE=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_SKIP_BUILD_RPATH=FALSE -DCMAKE_BUILD_WITH_INSTALL_RPATH=FALSE && make -j${CPU_NUM} && echo ******build release SUCCESS********
#	cp -rf $(CURDIR)/bin/resources $(CURDIR)/debian/tmp/usr/local/cuprumtest/bin/
	;;
"deb_package" )
	echo "build deb package"
	dpkg-buildpackage -b && echo ******build dev package SUCCESS********
	;;
"help" )
	echo "Usage:"
	echo "  ./build.sh [release] [4]"
	echo "  	use 4 processor to build release version"
	echo "  ./build.sh [deb_package]"
	echo "  	 build deb_package"
	echo "  ./build.sh [clean]"
	echo "  	clean out file before build"
;;
esac
popd

