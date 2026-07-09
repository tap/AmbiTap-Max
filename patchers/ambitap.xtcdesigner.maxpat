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
   1260,
   560
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
     "text": "xtc designer ↔ ambitap.xtc~",
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
     "text": "Drag a speaker (angle = span, radius = distance), slider = regularization — edits drive xtc~, and each change re-requests dumpfir so the plot shows the RUNNING object's designed filters."
    }
   },
   {
    "box": {
     "id": "obj-3",
     "maxclass": "v8ui",
     "patching_rect": [
      20,
      80,
      680,
      300
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "filename": "ambitap.xtcdesigner.js",
     "jsfile": "ambitap.xtcdesigner.js",
     "parameter_enable": 0
    }
   },
   {
    "box": {
     "id": "obj-4",
     "maxclass": "newobj",
     "patching_rect": [
      720,
      80,
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
      720,
      110,
      50,
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
     "id": "obj-6",
     "maxclass": "flonum",
     "patching_rect": [
      800,
      80,
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
     "id": "obj-7",
     "maxclass": "message",
     "patching_rect": [
      800,
      110,
      75,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "azimuth $1"
    }
   },
   {
    "box": {
     "id": "obj-8",
     "maxclass": "newobj",
     "patching_rect": [
      720,
      150,
      120,
      22
     ],
     "numinlets": 1,
     "numoutlets": 2,
     "outlettype": [
      "signal",
      "signal"
     ],
     "text": "ambitap.panbin~"
    }
   },
   {
    "box": {
     "id": "obj-9",
     "maxclass": "comment",
     "patching_rect": [
      850,
      152,
      260,
      20
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "binaural program material (pan it around)"
    }
   },
   {
    "box": {
     "id": "obj-10",
     "maxclass": "newobj",
     "patching_rect": [
      720,
      200,
      110,
      22
     ],
     "numinlets": 2,
     "numoutlets": 3,
     "outlettype": [
      "signal",
      "signal",
      ""
     ],
     "text": "ambitap.xtc~"
    }
   },
   {
    "box": {
     "id": "obj-11",
     "maxclass": "newobj",
     "patching_rect": [
      720,
      300,
      50,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "*~ 0.7"
    }
   },
   {
    "box": {
     "id": "obj-12",
     "maxclass": "newobj",
     "patching_rect": [
      790,
      300,
      50,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "signal"
     ],
     "text": "*~ 0.7"
    }
   },
   {
    "box": {
     "id": "obj-13",
     "maxclass": "ezdac~",
     "patching_rect": [
      720,
      340,
      44,
      44
     ],
     "numinlets": 2,
     "numoutlets": 0
    }
   },
   {
    "box": {
     "id": "obj-14",
     "maxclass": "newobj",
     "patching_rect": [
      480,
      400,
      32,
      22
     ],
     "numinlets": 1,
     "numoutlets": 1,
     "outlettype": [
      "bang"
     ],
     "text": "t b"
    }
   },
   {
    "box": {
     "id": "obj-15",
     "maxclass": "newobj",
     "patching_rect": [
      480,
      430,
      60,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      "bang"
     ],
     "text": "del 150"
    }
   },
   {
    "box": {
     "id": "obj-16",
     "maxclass": "message",
     "patching_rect": [
      480,
      460,
      55,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "dumpfir"
    }
   },
   {
    "box": {
     "id": "obj-17",
     "maxclass": "comment",
     "patching_rect": [
      560,
      430,
      300,
      34
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "param change → redesign → re-dump the FIRs into the plot"
    }
   },
   {
    "box": {
     "id": "obj-18",
     "maxclass": "newobj",
     "patching_rect": [
      620,
      460,
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
     "id": "obj-19",
     "maxclass": "toggle",
     "patching_rect": [
      900,
      200,
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
     "id": "obj-20",
     "maxclass": "message",
     "patching_rect": [
      900,
      232,
      70,
      22
     ],
     "numinlets": 2,
     "numoutlets": 1,
     "outlettype": [
      ""
     ],
     "text": "bypass $1"
    }
   },
   {
    "box": {
     "id": "obj-21",
     "maxclass": "comment",
     "patching_rect": [
      900,
      262,
      300,
      62
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "A/B honestly: xtc~ output sits ~12 dB below bypass by design (gain-ceiling makeup) — loudness-match before comparing (PERCEPTUAL-VERIFICATION bypass rule)."
    }
   },
   {
    "box": {
     "id": "obj-22",
     "maxclass": "comment",
     "patching_rect": [
      720,
      390,
      340,
      48
     ],
     "numinlets": 1,
     "numoutlets": 0,
     "text": "Listen on two loudspeakers at the drawn geometry, head in the sweet spot — headphones will not demonstrate crosstalk cancellation."
    }
   }
  ],
  "lines": [
   {
    "patchline": {
     "destination": [
      "obj-10",
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
      "obj-14",
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
      "obj-16",
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
      "obj-16",
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
      "obj-16",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-3",
      0
     ],
     "source": [
      "obj-10",
      2
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
      "obj-10",
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
      1
     ],
     "source": [
      "obj-8",
      1
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
      "obj-10",
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
      "obj-11",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-13",
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
      "obj-20",
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
      "obj-10",
      0
     ],
     "source": [
      "obj-20",
      0
     ]
    }
   }
  ]
 }
}
