{
 "patcher": {
  "fileversion": 1,
  "appversion": {
   "major": 9,
   "minor": 0,
   "revision": 0,
   "architecture": "x64",
   "modernui": 1
  },
  "classnamespace": "box",
  "rect": [
   40,
   60,
   1460,
   880
  ],
  "bglocked": 0,
  "openinpresentation": 0,
  "default_fontsize": 12,
  "default_fontface": 0,
  "default_fontname": "Arial",
  "gridonopen": 1,
  "gridsize": [
   15,
   15
  ],
  "gridsnaponopen": 1,
  "objectsnaponopen": 1,
  "statusbarvisible": 2,
  "toolbarvisible": 1,
  "boxes": [
   {
    "box": {
     "id": "obj-1",
     "maxclass": "comment",
     "patching_rect": [
      20,
      12,
      500,
      24
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "AmbiTap UI tour — every widget, one patch",
     "fontsize": 16
    }
   },
   {
    "box": {
     "id": "obj-2",
     "maxclass": "comment",
     "patching_rect": [
      20,
      38,
      900,
      48
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "Two panners drive encoders; the ball rotates the summed bus; grid~/energyvec~/mc.peakamp~ feed the heatmap, DOA, meters, and per-speaker layout levels; binaural~ monitors. Widgets are built into javascript/ by the CMake build (ambitap_ui target)."
    }
   },
   {
    "box": {
     "id": "obj-3",
     "maxclass": "comment",
     "patching_rect": [
      20,
      96,
      100,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "sources"
    }
   },
   {
    "box": {
     "id": "obj-4",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      118,
      60,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "noise~"
    }
   },
   {
    "box": {
     "id": "obj-5",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      148,
      55,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "*~ 0.15"
    }
   },
   {
    "box": {
     "id": "obj-6",
     "maxclass": "v8ui",
     "patching_rect": [
      20,
      180,
      190,
      190
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "filename": "ambitap.panner.js",
     "jsfile": "ambitap.panner.js",
     "parameter_enable": 0
    }
   },
   {
    "box": {
     "id": "obj-7",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      380,
      145,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      "multichannelsignal"
     ],
     "text": "ambitap.encode~ 3"
    }
   },
   {
    "box": {
     "id": "obj-8",
     "maxclass": "newobj",
     "patching_rect": [
      230,
      118,
      75,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "cycle~ 440"
    }
   },
   {
    "box": {
     "id": "obj-9",
     "maxclass": "newobj",
     "patching_rect": [
      230,
      148,
      50,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "*~ 0.1"
    }
   },
   {
    "box": {
     "id": "obj-10",
     "maxclass": "v8ui",
     "patching_rect": [
      230,
      180,
      190,
      190
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "filename": "ambitap.panner.js",
     "jsfile": "ambitap.panner.js",
     "parameter_enable": 0
    }
   },
   {
    "box": {
     "id": "obj-11",
     "maxclass": "newobj",
     "patching_rect": [
      230,
      380,
      145,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      "multichannelsignal"
     ],
     "text": "ambitap.encode~ 3"
    }
   },
   {
    "box": {
     "id": "obj-12",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      420,
      50,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "multichannelsignal"
     ],
     "text": "mc.+~"
    }
   },
   {
    "box": {
     "id": "obj-13",
     "maxclass": "comment",
     "patching_rect": [
      450,
      96,
      320,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "field rotation (yaw ring / tumble; dbl-click resets)"
    }
   },
   {
    "box": {
     "id": "obj-14",
     "maxclass": "v8ui",
     "patching_rect": [
      450,
      118,
      210,
      210
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "filename": "ambitap.rotation.js",
     "jsfile": "ambitap.rotation.js",
     "parameter_enable": 0
    }
   },
   {
    "box": {
     "id": "obj-15",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      460,
      140,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      "multichannelsignal"
     ],
     "text": "ambitap.rotate~ 3"
    }
   },
   {
    "box": {
     "id": "obj-16",
     "maxclass": "toggle",
     "patching_rect": [
      180,
      460,
      24,
      24
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      "int"
     ],
     "parameter_enable": 0
    }
   },
   {
    "box": {
     "id": "obj-17",
     "maxclass": "newobj",
     "patching_rect": [
      180,
      492,
      70,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "bang"
     ],
     "text": "qmetro 33"
    }
   },
   {
    "box": {
     "id": "obj-18",
     "maxclass": "comment",
     "patching_rect": [
      255,
      494,
      90,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "display clock"
    }
   },
   {
    "box": {
     "id": "obj-19",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      530,
      125,
      22
     ],
     "numinlets": 1,
     "numoutlets": 2,
     "outlettype": [
      "multichannelsignal",
      ""
     ],
     "text": "ambitap.grid~ 3"
    }
   },
   {
    "box": {
     "id": "obj-20",
     "maxclass": "v8ui",
     "patching_rect": [
      690,
      118,
      340,
      170
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "filename": "ambitap.heatmap.js",
     "jsfile": "ambitap.heatmap.js",
     "parameter_enable": 0
    }
   },
   {
    "box": {
     "id": "obj-21",
     "maxclass": "comment",
     "patching_rect": [
      690,
      292,
      380,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "soundfield heatmap (grid~ list; energy-vector crosshair below)"
    }
   },
   {
    "box": {
     "id": "obj-22",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      570,
      150,
      22
     ],
     "numinlets": 1,
     "numoutlets": 3,
     "outlettype": [
      "signal",
      "signal",
      "signal"
     ],
     "text": "ambitap.energyvec~"
    }
   },
   {
    "box": {
     "id": "obj-23",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      610,
      90,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "float"
     ],
     "text": "snapshot~ 33"
    }
   },
   {
    "box": {
     "id": "obj-24",
     "maxclass": "newobj",
     "patching_rect": [
      120,
      610,
      90,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "float"
     ],
     "text": "snapshot~ 33"
    }
   },
   {
    "box": {
     "id": "obj-25",
     "maxclass": "newobj",
     "patching_rect": [
      220,
      610,
      90,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "float"
     ],
     "text": "snapshot~ 33"
    }
   },
   {
    "box": {
     "id": "obj-26",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      650,
      115,
      22
     ],
     "numinlets": 4,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "pak vec 0. 0. 0."
    }
   },
   {
    "box": {
     "id": "obj-27",
     "maxclass": "v8ui",
     "patching_rect": [
      690,
      330,
      170,
      170
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "filename": "ambitap.doa.js",
     "jsfile": "ambitap.doa.js",
     "parameter_enable": 0
    }
   },
   {
    "box": {
     "id": "obj-28",
     "maxclass": "comment",
     "patching_rect": [
      690,
      504,
      340,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "energy-vector DOA (size/alpha = ||v|| vs recent peak)"
    }
   },
   {
    "box": {
     "id": "obj-29",
     "maxclass": "newobj",
     "patching_rect": [
      1060,
      118,
      110,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "mc.peakamp~ 50"
    }
   },
   {
    "box": {
     "id": "obj-30",
     "maxclass": "v8ui",
     "patching_rect": [
      1060,
      150,
      340,
      130
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "filename": "ambitap.meters.js",
     "jsfile": "ambitap.meters.js",
     "parameter_enable": 0
    }
   },
   {
    "box": {
     "id": "obj-31",
     "maxclass": "comment",
     "patching_rect": [
      1060,
      284,
      280,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "per-ACN meter bridge (16 ch at order 3)"
    }
   },
   {
    "box": {
     "id": "obj-32",
     "maxclass": "newobj",
     "patching_rect": [
      1060,
      330,
      170,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      "multichannelsignal"
     ],
     "text": "ambitap.decode~ 3 7.1.4"
    }
   },
   {
    "box": {
     "id": "obj-33",
     "maxclass": "newobj",
     "patching_rect": [
      1060,
      362,
      110,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "mc.peakamp~ 50"
    }
   },
   {
    "box": {
     "id": "obj-34",
     "maxclass": "v8ui",
     "patching_rect": [
      1060,
      394,
      230,
      210
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "filename": "ambitap.layout.js",
     "jsfile": "ambitap.layout.js",
     "parameter_enable": 0
    }
   },
   {
    "box": {
     "id": "obj-35",
     "maxclass": "newobj",
     "patching_rect": [
      1310,
      330,
      65,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      "bang"
     ],
     "text": "loadbang"
    }
   },
   {
    "box": {
     "id": "obj-36",
     "maxclass": "message",
     "patching_rect": [
      1310,
      362,
      85,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "preset 7.1.4"
    }
   },
   {
    "box": {
     "id": "obj-37",
     "maxclass": "comment",
     "patching_rect": [
      1060,
      608,
      340,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "speakers light with their decode feed; click one to inspect"
    }
   },
   {
    "box": {
     "id": "obj-38",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      700,
      150,
      22
     ],
     "numinlets": 1,
     "numoutlets": 2,
     "outlettype": [
      "signal",
      "signal"
     ],
     "text": "ambitap.binaural~ 3"
    }
   },
   {
    "box": {
     "id": "obj-39",
     "maxclass": "newobj",
     "patching_rect": [
      20,
      740,
      50,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "*~ 0.5"
    }
   },
   {
    "box": {
     "id": "obj-40",
     "maxclass": "newobj",
     "patching_rect": [
      90,
      740,
      50,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "*~ 0.5"
    }
   },
   {
    "box": {
     "id": "obj-41",
     "maxclass": "ezdac~",
     "patching_rect": [
      20,
      780,
      44,
      44
     ],
     "numinlets": 2,
     "numoutlets": 0
    }
   },
   {
    "box": {
     "id": "obj-42",
     "maxclass": "comment",
     "patching_rect": [
      160,
      782,
      200,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "binaural monitor (headphones)"
    }
   },
   {
    "box": {
     "id": "obj-43",
     "maxclass": "comment",
     "patching_rect": [
      450,
      560,
      560,
      34
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "remote surface: browser dashboard ?remote=ws://localhost:8090 → node ui/scripts/osc-bridge.mjs → here"
    }
   },
   {
    "box": {
     "id": "obj-44",
     "maxclass": "newobj",
     "patching_rect": [
      450,
      600,
      110,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "udpreceive 7500"
    }
   },
   {
    "box": {
     "id": "obj-45",
     "maxclass": "newobj",
     "patching_rect": [
      450,
      632,
      340,
      22
     ],
     "numinlets": 1,
     "numoutlets": 3,
     "outlettype": [
      "",
      "",
      ""
     ],
     "text": "route /ambitap/orientation /ambitap/source/1/direction"
    }
   },
   {
    "box": {
     "id": "obj-46",
     "maxclass": "newobj",
     "patching_rect": [
      450,
      668,
      105,
      22
     ],
     "numinlets": 1,
     "numoutlets": 3,
     "outlettype": [
      "float",
      "float",
      "float"
     ],
     "text": "unpack 0. 0. 0."
    }
   },
   {
    "box": {
     "id": "obj-47",
     "maxclass": "newobj",
     "patching_rect": [
      450,
      700,
      110,
      22
     ],
     "numinlets": 4,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "pak ypr 0. 0. 0."
    }
   },
   {
    "box": {
     "id": "obj-48",
     "maxclass": "newobj",
     "patching_rect": [
      620,
      668,
      85,
      22
     ],
     "numinlets": 1,
     "numoutlets": 2,
     "outlettype": [
      "float",
      "float"
     ],
     "text": "unpack 0. 0."
    }
   },
   {
    "box": {
     "id": "obj-49",
     "maxclass": "newobj",
     "patching_rect": [
      590,
      700,
      110,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "prepend azimuth"
    }
   },
   {
    "box": {
     "id": "obj-50",
     "maxclass": "newobj",
     "patching_rect": [
      590,
      732,
      115,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "prepend elevation"
    }
   },
   {
    "box": {
     "id": "obj-51",
     "maxclass": "comment",
     "patching_rect": [
      450,
      764,
      400,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "orientation → ball (drag-gated); source 1 direction → panner A"
    }
   }
  ],
  "lines": [
   {
    "patchline": {
     "destination": [
      "obj-5",
      0
     ],
     "source": [
      "obj-4",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-7",
      0
     ],
     "source": [
      "obj-5",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-7",
      0
     ],
     "source": [
      "obj-6",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-9",
      0
     ],
     "source": [
      "obj-8",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-11",
      0
     ],
     "source": [
      "obj-9",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-11",
      0
     ],
     "source": [
      "obj-10",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-12",
      0
     ],
     "source": [
      "obj-7",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-12",
      1
     ],
     "source": [
      "obj-11",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-15",
      0
     ],
     "source": [
      "obj-14",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-15",
      0
     ],
     "source": [
      "obj-12",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-17",
      0
     ],
     "source": [
      "obj-16",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-19",
      0
     ],
     "source": [
      "obj-17",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-19",
      0
     ],
     "source": [
      "obj-15",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-20",
      0
     ],
     "source": [
      "obj-19",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-22",
      0
     ],
     "source": [
      "obj-19",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-23",
      0
     ],
     "source": [
      "obj-22",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-24",
      0
     ],
     "source": [
      "obj-22",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-25",
      0
     ],
     "source": [
      "obj-22",
      2
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-26",
      1
     ],
     "source": [
      "obj-23",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-26",
      2
     ],
     "source": [
      "obj-24",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-26",
      3
     ],
     "source": [
      "obj-25",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-27",
      0
     ],
     "source": [
      "obj-26",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-20",
      0
     ],
     "source": [
      "obj-26",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-29",
      0
     ],
     "source": [
      "obj-19",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-30",
      0
     ],
     "source": [
      "obj-29",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-32",
      0
     ],
     "source": [
      "obj-19",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-33",
      0
     ],
     "source": [
      "obj-32",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-34",
      0
     ],
     "source": [
      "obj-33",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-36",
      0
     ],
     "source": [
      "obj-35",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-34",
      0
     ],
     "source": [
      "obj-36",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-38",
      0
     ],
     "source": [
      "obj-19",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-39",
      0
     ],
     "source": [
      "obj-38",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-40",
      0
     ],
     "source": [
      "obj-38",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-41",
      0
     ],
     "source": [
      "obj-39",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-41",
      1
     ],
     "source": [
      "obj-40",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-45",
      0
     ],
     "source": [
      "obj-44",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-46",
      0
     ],
     "source": [
      "obj-45",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-47",
      1
     ],
     "source": [
      "obj-46",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-47",
      2
     ],
     "source": [
      "obj-46",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-47",
      3
     ],
     "source": [
      "obj-46",
      2
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-14",
      0
     ],
     "source": [
      "obj-47",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-48",
      0
     ],
     "source": [
      "obj-45",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-49",
      0
     ],
     "source": [
      "obj-48",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-50",
      0
     ],
     "source": [
      "obj-48",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-6",
      0
     ],
     "source": [
      "obj-49",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-6",
      0
     ],
     "source": [
      "obj-50",
      0
     ]
    }
   }
  ]
 }
}
