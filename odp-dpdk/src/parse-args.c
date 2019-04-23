//
// Created by angelo on 10/04/18.
//

#include "parse-args.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <odp/helper/odph_api.h>


/**
 * Prinf usage information
 */
static void usage(char *progname)
{
    printf("\n"
                   "XTRA application.\n"
                   "Usage: %s OPTIONS\n"
                   "Mandatory OPTIONS:\n"
                   "  -n, --num-pktios Number of interfaces to add"
                   "  -s, --source-address IP source address\n"
                   "  -d, --dest-address IP destination address\n"
                   "  -D, --dest-port TCP destination port\n"
                   "  -m, --dest-mac Destination MAC address\n"
                   "\n"
                   "Optional OPTIONS\n"
                   "  -S, --source-port TCP Source port\n"
                   "  -h, --help		Display help and exit.\n"
                   "\n", progname
    );
}

/**
 * Parse text string representing an IPv4 address or subnet
 *
 * String is of the format "XXX.XXX.XXX.XXX(/W)" where
 * "XXX" is decimal value and "/W" is optional subnet length
 *
 * @param ipaddress  Pointer to IP address/subnet string to convert
 * @param addr       Pointer to return IPv4 address
 * @param mask       Pointer (optional) to return IPv4 mask
 *
 * @return 0 if successful else -1
 */
static inline
int parse_ipv4_string(char *ipaddress, uint32_t *addr, uint32_t *mask)
{
    int b[4];
    int qualifier = 32;
    int converted;

    if (strchr(ipaddress, '/')) {
        converted = sscanf(ipaddress, "%d.%d.%d.%d/%d",
                           &b[3], &b[2], &b[1], &b[0],
                           &qualifier);
        if (5 != converted)
            return -1;
    } else {
        converted = sscanf(ipaddress, "%d.%d.%d.%d",
                           &b[3], &b[2], &b[1], &b[0]);
        if (4 != converted)
            return -1;
    }

    if ((b[0] > 255) || (b[1] > 255) || (b[2] > 255) || (b[3] > 255))
        return -1;
    if (!qualifier || (qualifier > 32))
        return -1;

    *addr = (uint32_t) (b[0] | b[1] << 8 | b[2] << 16 | b[3] << 24);
    if (mask)
        *mask = (uint32_t) ~(0xFFFFFFFF & ((1ULL << (32 - qualifier)) - 1));

    return 0;
}


/**
 * Parse and store the command line arguments
 *
 * @param argc       argument count
 * @param argv[]     argument vector
 * @param appl_args  Store application arguments here
 */
void parse_args(int argc, char *argv[], appl_args_t *appl_args)
{
    int opt;
    int long_index;
    size_t len;
    int i;

    srand((unsigned int) time(NULL));
    uint16_t sport = (uint16_t) (40000 + rand()%20000);

    appl_args->src_port = sport;

    static const struct option longopts[] = {
            {"dest-mac", required_argument, NULL, 'm'},
            {"dest-address", required_argument, NULL, 'd'},
            {"source-address", required_argument, NULL, 's'},
            {"dest-port", required_argument, NULL, 'D'},
            {"source-port", required_argument, NULL, 'S'},
            {"num-pktios", required_argument, NULL, 'n'},
            {"help", no_argument, NULL, 'h'},
            {NULL, 0, NULL, 0}
    };

    static const char *shortopts = "m:d:s:D:S:n:h";

    if (argc < 9){
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    opterr = 0; // do not issue errors!!!

    while (1) {
        opt = getopt_long(argc, argv, shortopts, longopts, &long_index);

        if (opt == -1)
            break;	/* No more options */

        switch (opt) {
            case 'd':
                // parse destination address
                if (strlen(optarg)==0){
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                parse_ipv4_string(optarg, &appl_args->dst_addr, NULL);
                printf("dst ip addr: %s -> %u\n", optarg, appl_args->dst_addr);
                break;
            case 's':
                // parse source address
                // parse destination address
                if (strlen(optarg)==0){
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                parse_ipv4_string(optarg, &appl_args->src_addr, NULL);
                printf("src ip addr: %s -> %u\n", optarg, appl_args->src_addr);
                break;
            case 'm':
                len = strlen(optarg);
                if (len == 0) {
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                len += 1;	/* add room for '\0' */

                appl_args->dst_mac = malloc(len);
                if (appl_args->dst_mac == NULL) {
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                printf("MAC addr: %s\n", optarg);

                strcpy(appl_args->dst_mac, optarg);
                break;

            case 'h':
                usage(argv[0]);
                exit(EXIT_SUCCESS);
            case 'S':
                i = atoi(optarg);
                if (i < 1024 || i > 65535) {
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                else
                    appl_args->src_port = (uint16_t) i;
                break;
            case 'D':
                i = atoi(optarg);
                if (i < 1 || i > 65535) {
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                } else
                    appl_args->dst_port = (uint16_t) i;
                printf("destination port: %u\n",appl_args->dst_port);
                break;
            case 'n':
                i = atoi(optarg);
                if (i < 0 || i > 255) {
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                } else
                    appl_args->num_pktios = (uint8_t) i;
                printf("number of pktios: %u", appl_args->num_pktios);
            default:
                break;
        }
    }

    printf("source port: %u\n",appl_args->src_port);

    optind = 1;		/* reset 'extern optind' from the getopt lib */
}

/**
 * Print system and application info
 */
static void print_info(char *progname, appl_args_t *appl_args)
{
    printf("Running ODP XFSM appl: \"%s\"\n"
                   "-----------------\n",
           progname);
    printf("\n\n");
    fflush(NULL);
}

