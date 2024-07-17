job "iodev2sim" {
    datacenters = ["dc1"]

    group "iodev2sim_group" {
        network {
            port "iodev2" {} // use random port for redis
        }

        // register iodev2_sim_sim service in Consul
        service {
            name = "iodev2sim"
            port = "iodev2"
        }

        task "iodev2_sim" {
            driver = "raw_exec"

            config {
                command = "/bin/bash"
                args = ["-l", "-c", " fcfDevsimIodev --port ${NOMAD_PORT_iodev2} --cfg config/fcs/devsim/iodev2.cfg.yaml --use-ext-ip"]
            }
        }
    }
}
