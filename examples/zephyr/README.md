# Golioth Firmware SDK: Zephyr Support (beta)

Zephyr support in the Golioth Firmware SDK is in active development and
currently designated as "beta". This SDK supports all the features of
the Golioth platform, but is not yet as optimized as the Golioth Zephyr
SDK. We will update this page when this port is ready for general
availability. For devices going to production in 2023, please use the
[Golioth Zephyr SDK](https://github.com/golioth/golioth-zephyr-sdk).

We expect to reach general availability of Zephyr support in this SDK in
early 2024. If you are building a Zephyr based device with plans to go
to production in 2024 or later, you may want to consider beginning
development in this SDK to reduce the burden of transitioning later. The
SDK contains a set of examples designed to run using Zephyr RTOS.

API documentation: <https://firmware-sdk-docs.golioth.io/>

## Requirements:

* Install the latest version of the [Zephyr
  SDK](https://github.com/zephyrproject-rtos/sdk-ng/releases/latest).
* Install [Python](https://www.python.org/downloads/)
* Install [the West
  tool](https://docs.zephyrproject.org/latest/develop/west/install.html)


Most platforms are already supported with mainline [Zephyr
RTOS](https://www.zephyrproject.org/). This repository can be added to
any Zephyr based project as new West module. However, for making things
simple, this repository can also serve as a West manifest repo.

## Using the Golioth Firmware SDK as a West manifest repository

### Using with mainline Zephyr

Execute this command to download this repository together with all
dependencies:

```console
west init -m https://github.com/golioth/golioth-firmware-sdk.git --mf west-zephyr.yml
west update
cd modules/lib/golioth-firmware-sdk && git submodule update --init --recursive
```

Follow [Zephyr Getting
Started](https://docs.zephyrproject.org/latest/getting_started/index.html)
for details on how to setup Zephyr based projects.

### Using with Nordic's nRF Connect SDK

Execute this command to download this repository together with all
dependencies:

```console
west init -m https://github.com/golioth/golioth-firmware-sdk.git --mf west-ncs.yml
west update
cd modules/lib/golioth-firmware-sdk && git submodule update --init --recursive
```

Follow [nRF Connect SDK Getting
Started](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/gs_installing.html)
for details on how to setup nRF Connect SDK based projects.

### Adding the Golioth Firmware SDK to an existing Zephyr West project

Alternatively, add the following entry to the `west.yml` file of an
existing
[West](https://docs.zephyrproject.org/latest/west/index.html)
based project (e.g. Zephyr RTOS):

```yaml
# Golioth repository.
- name: golioth
  path: modules/lib/golioth-firmware-sdk
  revision: main
  url: https://github.com/golioth/golioth-firmware-sdk.git
  import: west-external.yml
```

and clone all repositories including that one by running:

```console
west update
cd modules/lib/golioth-firmware-sdk && git submodule update --init --recursive
```

# Sample applications

  - [Golioth Basics sample](golioth_basics/README.md)
  - [Golioth Certificate Provisioning sample](certificate_provisioning/README.md)
  - [Golioth DFU sample](dfu/README.md)
  - [Golioth Hello sample](hello/README.md)
  - [Golioth LightDB get sample](lightdb/get/README.md)
  - [Golioth LightDB observe sample](lightdb/observe/README.md)
  - [Golioth LightDB set sample](lightdb/set/README.md)
  - [Golioth LightDB Stream sample](lightdb_stream/README.md)
  - [Golioth Logging sample](logging/README.md)
  - [Golioth RPC sample](rpc/README.md)
  - [Golioth Settings sample](settings/README.md)

# Golioth Services

  - [Golioth Cloud](https://docs.golioth.io/cloud)
  - [LightDB
    state](https://docs.golioth.io/reference/protocols/coap/lightdb)
  - [LightDB
    Stream](https://docs.golioth.io/reference/protocols/coap/lightdb-stream)
  - [Logging](https://docs.golioth.io/reference/protocols/coap/logging)
  - [OTA](https://docs.golioth.io/reference/protocols/coap/ota)
  - [Authentication](https://docs.golioth.io/firmware/zephyr-device-sdk/authentication)
