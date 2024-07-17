!cfg.include config/fcf/devmgr/definitions/sensor.yaml:


iodev1: !cfg.type:Sensor
  identifier: PLC1
  prefix: MAIN.Iodev2
  mapfile: "config/fcf/devmgr/mapping/sensor.yaml"
  simulated: true
  dev_endpoint: not_defined
  
  sim_endpoint: opc.tcp://{{ range service "iodev2sim" }}{{ .Address }}:{{ .Port }}{{ end }}
  
  fits_prefix: "SENSOR2"
  channels: [
  {
  name: ch7,
  description: "This is the channel 7",
  fits_prefix: "CH7 STAT",
  type: DI,
  unit: mm,
  map: di7,
  named_values: [
  {
        name: 'HIGH',
        value: '1'
       },
       {
        name: 'LOW',
        value: '0'
        }
       ]
  },
  {
  name: ch8,
  description: "channel8",
  fits_prefix: "CH8 STAT",
  type: DI,
  unit: dd,
  map: di8,
  named_values: [
  {
        name: 'ON',
        value: '1'
       },
       {
        name: 'OFF',
        value: '0'
        }
       ]
  },
  {
  name: ch9,
  description: "channel9",
  fits_prefix: "CH9 STAT",
  type: DI,
  unit: df,
  map: di9
  },
  {
  name: ch10,
  description: "channel10",
  fits_prefix: "CH10 VAL",
  type: AI,
  unit: volts,
  map: ai10
  },
  {
  name: ch11,
  description: "channel11",
  fits_prefix: "CH11 VAL",
  type: AI,
  unit: degrees,
  map: ai11
  },
  {
  name: ch12,
  description: "channel12",
  fits_prefix: "CH12 VAL",
  type: AI,
  map: ai12
  },
  ]

