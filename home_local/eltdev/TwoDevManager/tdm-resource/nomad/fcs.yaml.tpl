!cfg.include config/fcf/devmgr/definitions/server.yaml:

server: !cfg.type:FcfServer
    server_id       : 'fcs'

    req_endpoint    : "zpb.rr://{{ range service "fcs-req" }}{{ .Address }}:{{ .Port }}{{ end }}/"
    pub_endpoint    : "zpb.ps://{{ range service "fcs-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/"

    db_timeout      : 2000
    scxml           : "config/fcf/devmgr/server/sm.xml"
    dictionaries    : ['dictionary/dit/stddid/primary.did.yaml', 'dictionary/fcf/devmgr/server/fcf.did']
    fits_prefix     : "INS1"
    oldb_prefix     : 'elt/tdm'
    req_timeout     : 120000
    log_properties  : "config/fcs/server/log_properties.cfg"
    version         : '0.0.0'
    gui: {
          containers: [
           {
             name: 'Devices',
             position: 'LEFT'
           },
           {
             name: 'Sensors',
             position: 'RIGHT'
           }
         ]
       }
    devices         : [
    {
    name: 'shutter1',
    type: Shutter,
    cfgfile: "local/shutter1.yaml"
    },
    {
    name: 'motor1',
    type: Motor,
    cfgfile: "local/motor1.yaml"
    },
    {
    name: 'iodev1',
    type: IODev,
    cfgfile: "local/iodev1.yaml",
    widget: {
          container: "Sensors"
        }
    },
    {
    name: 'mos1',
    type: Mos,
    cfgfile: "local/mos1.yaml",
    widget: {
          custom: {
             library: "tdm.fcs.pyfcsgui.wdglib.taurusmos",
             class: "TaurusMos"
          },
          container: "Devices"
        }
    }
    ]


