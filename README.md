# XFSMs for TRAnsport

This repository contains the source code and related files for the XFSMs for TRAnsport project. The directories are organized as follows: 

- [ns3](ns3) contains the patched version of NS3 simulator to run TCP simulations with the features provided by XTRA;
- [odp-xfsm](odp-xfsm) contains the XTRA implementation built upon the OpenDataPlane and DPDK frameworks;
- [syn-proxy-traces](syn-proxy-traces) are the traces captured while performing a SYN proxy application implemented in XTRA;
- [xtrac](xtrac) is the compiler written in Java that produces a JSON representation of the XTRALANG code;
- [xtralang-sources](xtralang-sources) comprises four Transport Layer example applications written in _xtralang_;
- [hw-vhdl](hw-vhdl) contains the VHDL code implementing XTRA and targeting a NetFPGA SUME Smart NIC. 
