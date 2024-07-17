job "tdmredis" {
    datacenters = ["dc1"]
    
    group "tdmredis_group" {
        network {
            port "redis" {} // use random port for redis
        }

        // register redis service in Consul
        service {
            name = "tdmredis"
            port = "redis"
        }

        task "tdmredis" {
            driver = "raw_exec"

            config {
                command = "/opt/anaconda38/bin/redis-server"
                args = ["--protected-mode", "no", "--port", "${NOMAD_PORT_redis}"]
            }
        }
    }
}
