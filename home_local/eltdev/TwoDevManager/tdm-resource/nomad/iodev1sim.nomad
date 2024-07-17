job "iodev1sim" {
    datacenters = ["dc1"]

    group "iodev1sim_group" {
        network {
            port "iodev1" {} // use random port for redis
        }

        // register iodev1_sim_sim service in Consul
        service {
            name = "iodev1sim"
            port = "iodev1"
        }

        task "iodev1_sim" {
            driver = "raw_exec"

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " fcfDevsimIodev --port ${NOMAD_PORT_iodev1} --cfg config/fcs/devsim/iodev1.cfg.yaml --use-ext-ip"]
            }
        }
    }
}
