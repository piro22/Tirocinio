job "hlcc" {
  datacenters       = ["dc1"]

  group "telifsim_group" {
    network {
      port "telifsim_zmq" {}   // random port for req/rep
    }

    // register pub/sub service in Consul
    service {
      name = "telifsim"
      port = "telifsim_zmq"
    }
    task "telifsim_task" {
      driver        = "raw_exec"
      template {
        source      = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/telifsim-ins-config.yaml.tpl"
        destination = "local/config.yaml"
      }
      config {
        command     = "/bin/bash"
        args        = [
          "-l",
          "-c",
          " telifsim -c local/config.yaml"
        ]
      }
    }
  }


  group "eltpksim_group" {
    network {
      port "eltpksim_zmq" {}   // random port for req/rep
    }

    // register pub/sub service in Consul
    service {
      name = "eltpksim"
      port = "eltpksim_zmq"
    }
    task "eltpksim_task" {
      driver        = "raw_exec"
      template {
        source      = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/eltpksim-ins-config.yaml.tpl"
        destination = "local/config.yaml"
      }
      config {
        command     = "/bin/bash"
        args        = [
          "-l",
          "-c",
          " eltpksim -c local/config.yaml"
        ]
      }
    }
  }


  group "telmon_group" {
    network {
      port "telmon_zmq" {}   // random port for req/rep
    }

    // register pub/sub service in Consul
    service {
      name = "telmon"
      port = "telmon_zmq"
    }
    task "telmon_task" {
      driver        = "raw_exec"
      template {
        source      = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad/telmon-ins-config.yaml.tpl"
        destination = "local/config.yaml"
      }
      config {
        command     = "/bin/bash"
        args        = [
          "-l",
          "-c",
          " telmon -c local/config.yaml"
        ]
      }
    }
  }

group "seqserver_group" {
    network {
      port "seqserver_zmq" {}   // random port for req/rep
    }

    task "seqserver_task" {
      driver            = "raw_exec"
      service {
        name            = "seqserver"
        port            = "seqserver_zmq"
      }
      config {
        command     = "/bin/bash"
        args        = [
          "-l",
          "-c",
          "seqtool server --address ${NOMAD_ADDR_seqserver_zmq}"
        ]
      }
    }
  }

}

