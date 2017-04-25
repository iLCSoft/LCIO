# Go example for LCIO

This is a simple example modeled after the `C++/rootDict` one.

You need a [Go](https://golang.org) toolchain.

And the [go-hep.org/x/hep/lcio](https://godoc.org/go-hep.org/x/hep/lcio) package installed.
This can be done like so:

```sh
$> go get go-hep.org/x/hep/lcio
```

Once the `lcio` package is installed, you can run the `read-event` example like so:

```sh
$> cd examples/go
$> go run ./read-event/main.go ../cpp/rootDict/DST01-06_ppr004_bbcsdu.slcio
lcio-ex-read-event: read 50 events from file "../cpp/rootDict/DST01-06_ppr004_bbcsdu.slcio"

$> open out.png
```

