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
     "text": "Hearing in Three Dimensions \u2014 05 \u00b7 Distance",
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
     "text": "A ticking source with the full distance chain. Sweep the meters; toggle each stage. Headphones. (Book: ch. 10.)",
     "id": "obj-2"
    }
   },
   {
    "box": {
     "maxclass": "toggle",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      70.0,
      24.0,
      24.0
     ],
     "outlettype": [
      "int"
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
      100.0,
      80.0,
      22.0
     ],
     "text": "metro 250",
     "outlettype": [
      "bang"
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
      136.0,
      50.0,
      22.0
     ],
     "text": "click~",
     "outlettype": [
      "signal"
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
      40.0,
      172.0,
      60.0,
      22.0
     ],
     "text": "*~ 0.5",
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
      40.0,
      230.0,
      140.0,
      22.0
     ],
     "text": "ambitap.encode~ 3",
     "outlettype": [
      "multichannelsignal"
     ],
     "id": "obj-7"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      200.0,
      200.0,
      90.0,
      22.0
     ],
     "text": "azimuth 0.8",
     "outlettype": [
      ""
     ],
     "id": "obj-8"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      330.0,
      150.0,
      22.0
     ],
     "text": "ambitap.distance~ 3",
     "outlettype": [
      "multichannelsignal"
     ],
     "id": "obj-9"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      40.0,
      430.0,
      145.0,
      22.0
     ],
     "text": "ambitap.binaural~ 3",
     "outlettype": [
      "signal",
      "signal"
     ],
     "id": "obj-10"
    }
   },
   {
    "box": {
     "maxclass": "ezdac~",
     "numinlets": 2,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      500.0,
      45.0,
      45.0
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
      420.0,
      70.0,
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
      486.0,
      72.0,
      210.0,
      20.0
     ],
     "text": "distance (meters, try 0.3 \u2013 40)",
     "id": "obj-13"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      420.0,
      100.0,
      70.0,
      22.0
     ],
     "text": "distance $1",
     "outlettype": [
      ""
     ],
     "id": "obj-14"
    }
   },
   {
    "box": {
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      420.0,
      140.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ],
     "id": "obj-15"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      486.0,
      142.0,
      210.0,
      20.0
     ],
     "text": "reference distance (default 1 m)",
     "id": "obj-16"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      420.0,
      170.0,
      150.0,
      22.0
     ],
     "text": "reference_distance $1",
     "outlettype": [
      ""
     ],
     "id": "obj-17"
    }
   },
   {
    "box": {
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      420.0,
      210.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
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
      486.0,
      212.0,
      210.0,
      20.0
     ],
     "text": "1/r exponent (1 = physics, 0.5 = cinematic)",
     "id": "obj-19"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      420.0,
      240.0,
      89.0,
      22.0
     ],
     "text": "attenuation $1",
     "outlettype": [
      ""
     ],
     "id": "obj-20"
    }
   },
   {
    "box": {
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      420.0,
      280.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ],
     "id": "obj-21"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      486.0,
      282.0,
      210.0,
      20.0
     ],
     "text": "air absorption depth",
     "id": "obj-22"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      420.0,
      310.0,
      130.0,
      22.0
     ],
     "text": "air_absorption $1",
     "outlettype": [
      ""
     ],
     "id": "obj-23"
    }
   },
   {
    "box": {
     "maxclass": "toggle",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      720.0,
      70.0,
      24.0,
      24.0
     ],
     "outlettype": [
      "int"
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
      752.0,
      73.0,
      220.0,
      20.0
     ],
     "text": "doppler on/off (default on)",
     "id": "obj-25"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      720.0,
      100.0,
      70.0,
      22.0
     ],
     "text": "doppler $1",
     "outlettype": [
      ""
     ],
     "id": "obj-26"
    }
   },
   {
    "box": {
     "maxclass": "toggle",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      720.0,
      140.0,
      24.0,
      24.0
     ],
     "outlettype": [
      "int"
     ],
     "id": "obj-27"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      752.0,
      143.0,
      220.0,
      20.0
     ],
     "text": "near-field compensation on/off",
     "id": "obj-28"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      720.0,
      170.0,
      70.0,
      22.0
     ],
     "text": "nfc $1",
     "outlettype": [
      ""
     ],
     "id": "obj-29"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      570.0,
      920.0,
      33.0
     ],
     "text": "Experiments: (1) sweep distance slowly with everything on; (2) automate a fast 2 m -> 30 m pass and toggle doppler; (3) creep inside 1 m and A/B nfc. A dry chain still sounds like an anechoic void \u2014 rooms are ch. 11.",
     "id": "obj-30"
    }
   }
  ],
  "lines": [
   {
    "patchline": {
     "destination": [
      "obj-14",
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
      "obj-18",
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
      "obj-21",
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
      "obj-29",
      0
     ],
     "source": [
      "obj-27",
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
      "obj-6",
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
      "obj-7",
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
      "obj-9",
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
      "obj-14",
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
      "obj-17",
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
      "obj-20",
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
      "obj-23",
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
      "obj-26",
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
      "obj-29",
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
      "obj-11",
      1
     ],
     "source": [
      "obj-10",
      1
     ]
    }
   }
  ]
 }
}
