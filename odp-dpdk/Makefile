SYS=-isystem /opt/odp-dpdk/dpdk/install/include/dpdk -msse4.2
INCLUDES=-I. -I/opt/odp-dpdk/example -I/opt/odp-dpdk/include -I/opt/odp-dpdk/include -I/opt/odp-dpdk/include/odp/arch/x86_64-linux  -I/opt/odp-dpdk/helper/include
CFLAGS=-g -O2 -std=c99 -mcx16
LIBS=-Wl,--whole-archive -Wl,-lrte_pmd_af_packet -Wl,-lrte_pmd_ark -Wl,-lrte_pmd_avp -Wl,-lrte_pmd_bnxt -Wl,-lrte_pmd_bond -Wl,-lrte_pmd_crypto_scheduler -Wl,-lrte_pmd_cxgbe -Wl,-lrte_pmd_e1000 -Wl,-lrte_pmd_ena -Wl,-lrte_pmd_enic -Wl,-lrte_pmd_failsafe -Wl,-lrte_pmd_fm10k -Wl,-lrte_pmd_i40e -Wl,-lrte_pmd_ixgbe -Wl,-lrte_pmd_kni -Wl,-lrte_pmd_lio -Wl,-lrte_pmd_nfp -Wl,-lrte_pmd_null -Wl,-lrte_pmd_null_crypto -Wl,-lrte_pmd_octeontx -Wl,-lrte_pmd_octeontx_ssovf -Wl,-lrte_pmd_openssl -Wl,-lrte_pmd_pcap -Wl,-lrte_pmd_qede -Wl,-lrte_pmd_ring -Wl,-lrte_pmd_sfc_efx -Wl,-lrte_pmd_skeleton_event -Wl,-lrte_pmd_softnic -Wl,-lrte_pmd_sw_event -Wl,-lrte_pmd_tap -Wl,-lrte_pmd_thunderx_nicvf -Wl,-lrte_pmd_vhost -Wl,-lrte_pmd_virtio -Wl,-lrte_pmd_vmxnet3_uio -Wl,--no-whole-archive -ldpdk -ldl -lpthread -lnuma -lm -lcrypto -lpcap  -L/opt/odp-dpdk/lib /opt/odp-dpdk/lib/.libs/libodp-linux.a -L/opt/odp-dpdk/dpdk/install/lib /opt/odp-dpdk/lib/.libs/libodphelper.a /opt/odp-dpdk/lib/.libs/libodp-linux.a -latomic -lconfig -lrt -ldpdk -ldl -lpthread -lnuma -lm -lcrypto -lpcap -pthread

default:
	#gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/test-lookup-timer.o test/test-lookup-timer.c &&\
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/parse-args.o src/parse-args.c &&\
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/session.o src/session.c &&\
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/pktio.o src/pktio.c &&\
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/tcp-ops.o src/tcp-ops.c &&\
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/xfsm.o src/xfsm.c &&\
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/pkt-parser.o src/pkt-parser.c &&\
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/xfsm-timer.o src/xfsm-timer.c &&\
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/cJSON.o src/cJSON.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/loader.o src/loader.c &&\
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/test-loader.o test/test-loader.c &&\
    gcc -DHAVE_CONFIG_H $(INCLUDES) $(CFLAGS) -o loader build/cJSON.o build/loader.o build/test-loader.o build/pkt-parser.o build/parse-args.o build/pktio.o build/tcp-ops.o build/xfsm.o build/xfsm-timer.o build/session.o $(LIBS)

udp:
	gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/parse-args.o src/parse-args.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/session.o src/session.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/pktio.o src/pktio.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/tcp-ops.o src/tcp-ops.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/xfsm.o src/xfsm.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/pkt-parser.o src/pkt-parser.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/xfsm-timer.o src/xfsm-timer.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/cJSON.o src/cJSON.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/loader.o src/loader.c &&\
    gcc -DHAVE_CONFIG_H $(ISYS) $(INCLUDES) $(CFLAGS) -c -o build/test-udp.o test/test-udp.c &&\
    gcc -DHAVE_CONFIG_H $(INCLUDES) $(CFLAGS) -o build/udp build/cJSON.o build/loader.o build/test-udp.o build/pkt-parser.o build/parse-args.o build/pktio.o build/tcp-ops.o build/xfsm.o build/xfsm-timer.o build/session.o $(LIBS)
