
job "dpm" {
    datacenters = ["dc1"]

    group "dpm_group" {
        network {
            port "pub" {}   // random port for pub/sub
            port "req" {}   // random port for req/rep
        }

        // register pub/sub service in Consul
        service {
            name = "dpm-pub"
            port = "pub"
        }

        // register req/rep service in Consul
        service {
            name = "dpm-req"
            port = "req"
        }

        task "dpm" {
            driver = "raw_exec"

            config {
                command = "/bin/bash"
                # note: nomad is not exporting variables NOMAD_ADDR_<label> for some reason
                args = ["-l", "-c", " daqDpmServer --rr-uri zpb.rr://*:${NOMAD_PORT_req} --ps-uri zpb.ps://*:${NOMAD_PORT_pub} -c config/daq/dpm.yaml"]
            }

        }
    }
}

