{
 "patcher": {
  "fileversion": 1,
  "appversion": {
   "major": 8,
   "minor": 0,
   "revision": 0,
   "architecture": "x64",
   "modernui": 1
  },
  "classnamespace": "box",
  "rect": [
   60.0,
   80.0,
   980.0,
   680.0
  ],
  "bglocked": 0,
  "openinpresentation": 0,
  "default_fontsize": 12.0,
  "default_fontface": 0,
  "default_fontname": "Arial",
  "gridonopen": 1,
  "gridsize": [
   15.0,
   15.0
  ],
  "gridsnaponopen": 1,
  "objectsnaponopen": 1,
  "statusbarvisible": 2,
  "toolbarvisible": 1,
  "boxes": [
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      12.0,
      700.0,
      24.0
     ],
     "text": "Hearing in Three Dimensions \u2014 07 \u00b7 Decoding to real speakers",
     "fontsize": 16.0,
     "id": "obj-1"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      40.0,
      930.0,
      20.0
     ],
     "text": "An orbiting source into a 5.1 decode with the three constructions on messages, plus a binaural stand-in. Raise ONE monitor gain. (Book: ch. 12.)",
     "id": "obj-2"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      100.0,
      50.0,
      22.0
     ],
     "text": "pink~",
     "outlettype": [
      "signal"
     ],
     "id": "obj-3"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      136.0,
      60.0,
      22.0
     ],
     "text": "*~ 0.25",
     "outlettype": [
      "signal"
     ],
     "id": "obj-4"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      240.0,
      140.0,
      22.0
     ],
     "text": "ambitap.encode~ 3",
     "outlettype": [
      "multichannelsignal"
     ],
     "id": "obj-5"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      230.0,
      100.0,
      90.0,
      22.0
     ],
     "text": "phasor~ 0.05",
     "outlettype": [
      "signal"
     ],
     "id": "obj-6"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      230.0,
      134.0,
      90.0,
      22.0
     ],
     "text": "snapshot~ 30",
     "outlettype": [
      "float"
     ],
     "id": "obj-7"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      230.0,
      168.0,
      140.0,
      22.0
     ],
     "text": "expr $f1 * 6.2831853",
     "outlettype": [
      ""
     ],
     "id": "obj-8"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      230.0,
      202.0,
      90.0,
      22.0
     ],
     "text": "azimuth $1",
     "outlettype": [
      ""
     ],
     "id": "obj-9"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      340.0,
      200.0,
      22.0
     ],
     "text": "ambitap.decode~ 3 surround_5_1",
     "outlettype": [
      "multichannelsignal"
     ],
     "id": "obj-10"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      420.0,
      70.0,
      22.0
     ],
     "text": "mc.*~ 0.",
     "outlettype": [
      "multichannelsignal"
     ],
     "id": "obj-11"
    }
   },
   {
    "box": {
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      130.0,
      420.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ],
     "id": "obj-12"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      196.0,
      423.0,
      180.0,
      20.0
     ],
     "text": "speaker gain (0 -> 1)",
     "id": "obj-13"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      456.0,
      120.0,
      22.0
     ],
     "text": "mc.dac~ 1 2 3 4 5",
     "outlettype": [],
     "id": "obj-14"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      482.0,
      340.0,
      20.0
     ],
     "text": "channel order: L, R, C, LS, RS (no LFE \u2014 book ch. 17)",
     "id": "obj-15"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      430.0,
      100.0,
      170.0,
      22.0
     ],
     "text": "decoder_type mode_match",
     "outlettype": [
      ""
     ],
     "id": "obj-16"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      430.0,
      130.0,
      140.0,
      22.0
     ],
     "text": "decoder_type allrad",
     "outlettype": [
      ""
     ],
     "id": "obj-17"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      430.0,
      160.0,
      130.0,
      22.0
     ],
     "text": "decoder_type epad",
     "outlettype": [
      ""
     ],
     "id": "obj-18"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      610.0,
      128.0,
      330.0,
      20.0
     ],
     "text": "switch MID-ORBIT \u2014 rebuilds crossfade in click-free",
     "id": "obj-19"
    }
   },
   {
    "box": {
     "maxclass": "toggle",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      430.0,
      200.0,
      24.0,
      24.0
     ],
     "outlettype": [
      "int"
     ],
     "id": "obj-20"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      462.0,
      203.0,
      220.0,
      20.0
     ],
     "text": "max-rE weighting (usually better on)",
     "id": "obj-21"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      430.0,
      230.0,
      70.0,
      22.0
     ],
     "text": "max_re $1",
     "outlettype": [
      ""
     ],
     "id": "obj-22"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      500.0,
      340.0,
      145.0,
      22.0
     ],
     "text": "ambitap.binaural~ 3",
     "outlettype": [
      "signal",
      "signal"
     ],
     "id": "obj-23"
    }
   },
   {
    "box": {
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      660.0,
      340.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ],
     "id": "obj-24"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      726.0,
      342.0,
      210.0,
      20.0
     ],
     "text": "binaural stand-in gain (0 -> 1)",
     "id": "obj-25"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      660.0,
      370.0,
      70.0,
      22.0
     ],
     "text": "volume $1",
     "outlettype": [
      ""
     ],
     "id": "obj-26"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 0,
     "patching_rect": [
      500.0,
      430.0,
      70.0,
      22.0
     ],
     "text": "dac~ 1 2",
     "outlettype": [],
     "id": "obj-27"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      560.0,
      920.0,
      47.0
     ],
     "text": "The protocol (book fig.): orbit on mode_match and hear loudness swing through the rear gap; switch to allrad and hear it level out (and the rear soften); toggle max_re on each; park the source ON a speaker (azimuth 110 deg) where the constructions nearly agree.",
     "id": "obj-28"
    }
   }
  ],
  "lines": [
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
      "obj-8",
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
      "obj-22",
      0
     ],
     "source": [
      "obj-20",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-26",
      0
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
      "obj-4",
      0
     ],
     "source": [
      "obj-3",
      0
     ]
    }
   },
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
      "obj-5",
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
      "obj-10",
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
      "obj-23",
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
      "obj-10",
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
      "obj-10",
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
      "obj-10",
      0
     ],
     "source": [
      "obj-18",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-10",
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
      "obj-11",
      1
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
      "obj-14",
      0
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
      "obj-23",
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
      "obj-27",
      0
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
      "obj-27",
      1
     ],
     "source": [
      "obj-23",
      1
     ]
    }
   }
  ]
 }
}
