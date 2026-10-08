# 🌐 ft_traceroute

*A network diagnostic tool written in C — 42 School Project.*

## 📖 About

**ft_traceroute** is a reimplementation of the Unix `traceroute` command, developed as part of the **42 School curriculum**.

The goal of this project is to understand how packets travel across IP networks by tracing the route between a local machine and a destination host.

By manipulating **IPv4, TTL (Time To Live), sockets, and ICMP messages**, this project explores low-level network programming and packet routing.

## ⚙️ Features

- IPv4 address and hostname support
- Detection of intermediate routers (hops)
- Round-trip time (RTT) measurements
- TTL-based network path discovery
- Fully implemented in C
- `--help` option for usage information
- Makefile for compilation

## 🛠️ Compilation

Clone the repository and compile the project:

```bash
git clone https://github.com/<username>/ft_traceroute.git
cd ft_traceroute
make
```

This generates the executable:

```bash
./ft_traceroute
```

## 🚀 Usage

Trace the route to an IPv4 address:

```bash
./ft_traceroute 8.8.8.8
```

Trace the route to a hostname:

```bash
./ft_traceroute google.com
```

Display help:

```bash
./ft_traceroute --help
```

**Note:** Depending on the socket implementation and system configuration, root privileges may be required.

## 🧠 How It Works

The traceroute mechanism relies on the **Time To Live (TTL)** field of IPv4 packets.

1. Send a probe packet with an initial TTL of 1.
2. The first router decreases the TTL to 0 and may return an ICMP Time Exceeded message.
3. Record the responding router's IP address and the round-trip time.
4. Increase the TTL and send another probe.
5. Repeat until the destination is reached or the hop limit is exceeded.

This process reveals the intermediate routers traversed by packets on their way to the destination.

## 📋 Project Requirements

- Language: C
- Executable: `ft_traceroute`
- Compilation: Makefile
- Mandatory option: `--help`
- Protocol: IPv4
- Destination: IPv4 address or hostname
- Socket restriction: No `O_NONBLOCK` mode
- Mandatory dependencies: C standard library (libC), with personal libraries permitted
- Implementation: The system's existing `traceroute` command must not be called

## 🎓 Learning Objectives

Through this project, the main concepts explored are:

- Network programming in C
- IPv4 packet structure and routing
- ICMP protocol and error messages
- TTL manipulation
- Socket programming
- DNS and hostname resolution
- Network latency measurements

## 👩‍💻 Author

Developed as part of the **42 School** curriculum.
