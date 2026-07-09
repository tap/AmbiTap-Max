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
   60,
   80,
   920,
   620
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
      14,
      400,
      22
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "ambitap.grid~",
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
      660,
      34
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "Soundfield energy heatmap analysis: the HOA bus passes through unchanged; bang emits the smoothed equirectangular grid as a list for the AmbiTap heatmap widget (v8ui, from the library repo's ui/)."
    }
   },
   {
    "box": {
     "id": "obj-3",
     "maxclass": "newobj",
     "patching_rect": [
      40,
      96,
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
     "id": "obj-4",
     "maxclass": "newobj",
     "patching_rect": [
      40,
      126,
      55,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "*~ 0.2"
    }
   },
   {
    "box": {
     "id": "obj-5",
     "maxclass": "newobj",
     "patching_rect": [
      40,
      160,
      150,
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
     "id": "obj-6",
     "maxclass": "comment",
     "patching_rect": [
      200,
      162,
      140,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "(moving HOA source)"
    }
   },
   {
    "box": {
     "id": "obj-7",
     "maxclass": "newobj",
     "patching_rect": [
      220,
      96,
      90,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "phasor~ 0.05"
    }
   },
   {
    "box": {
     "id": "obj-8",
     "maxclass": "newobj",
     "patching_rect": [
      220,
      126,
      70,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "*~ 6.2832"
    }
   },
   {
    "box": {
     "id": "obj-9",
     "maxclass": "newobj",
     "patching_rect": [
      320,
      126,
      90,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "float"
     ],
     "text": "snapshot~ 50"
    }
   },
   {
    "box": {
     "id": "obj-10",
     "maxclass": "newobj",
     "patching_rect": [
      320,
      156,
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
     "id": "obj-11",
     "maxclass": "flonum",
     "patching_rect": [
      450,
      96,
      60,
      22
     ],
     "numinlets": 1,
     "numoutlets": 2,
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-12",
     "maxclass": "message",
     "patching_rect": [
      450,
      126,
      85,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "elevation $1"
    }
   },
   {
    "box": {
     "id": "obj-13",
     "maxclass": "newobj",
     "patching_rect": [
      40,
      210,
      130,
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
     "id": "obj-14",
     "maxclass": "toggle",
     "patching_rect": [
      190,
      178,
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
     "id": "obj-15",
     "maxclass": "newobj",
     "patching_rect": [
      190,
      210,
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
     "id": "obj-16",
     "maxclass": "comment",
     "patching_rect": [
      265,
      212,
      140,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "bang at display rate"
    }
   },
   {
    "box": {
     "id": "obj-17",
     "maxclass": "comment",
     "patching_rect": [
      40,
      240,
      430,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "← HOA passthrough: insert inline like a meter (→ decode~ / binaural~)"
    }
   },
   {
    "box": {
     "id": "obj-18",
     "maxclass": "v8ui",
     "patching_rect": [
      40,
      270,
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
     "id": "obj-19",
     "maxclass": "comment",
     "patching_rect": [
      40,
      444,
      380,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "the heatmap widget consumes the grid list directly (no Jitter)"
    }
   },
   {
    "box": {
     "id": "obj-20",
     "maxclass": "message",
     "patching_rect": [
      480,
      210,
      105,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "azimuth_steps 16"
    }
   },
   {
    "box": {
     "id": "obj-21",
     "maxclass": "message",
     "patching_rect": [
      480,
      240,
      105,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "azimuth_steps 32"
    }
   },
   {
    "box": {
     "id": "obj-22",
     "maxclass": "flonum",
     "patching_rect": [
      480,
      280,
      60,
      22
     ],
     "numinlets": 1,
     "numoutlets": 2,
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-23",
     "maxclass": "message",
     "patching_rect": [
      480,
      310,
      115,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "smoothing_time $1"
    }
   },
   {
    "box": {
     "id": "obj-24",
     "maxclass": "comment",
     "patching_rect": [
      600,
      312,
      260,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "per-direction smoothing, ms (default 200)"
    }
   },
   {
    "box": {
     "id": "obj-25",
     "maxclass": "flonum",
     "patching_rect": [
      480,
      340,
      60,
      22
     ],
     "numinlets": 1,
     "numoutlets": 2,
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-26",
     "maxclass": "message",
     "patching_rect": [
      480,
      370,
      110,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "dynamic_range $1"
    }
   },
   {
    "box": {
     "id": "obj-27",
     "maxclass": "comment",
     "patching_rect": [
      600,
      372,
      280,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "dB below the peak mapped to 0..1 (default 40)"
    }
   }
  ],
  "lines": [
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
      "obj-5",
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
      "obj-11",
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
      "obj-12",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-13",
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
      "obj-13",
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
      "obj-18",
      0
     ],
     "source": [
      "obj-13",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-13",
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
      "obj-13",
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
      "obj-13",
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
      "obj-26",
      0
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
      "obj-13",
      0
     ],
     "source": [
      "obj-26",
      0
     ]
    }
   }
  ]
 }
}
