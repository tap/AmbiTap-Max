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
   920.0,
   620.0
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
     "id": "obj-1",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      14.0,
      400.0,
      22.0
     ],
     "text": "ambitap.panbin~",
     "fontsize": 16.0
    }
   },
   {
    "box": {
     "id": "obj-2",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      38.0,
      680.0,
      40.0
     ],
     "text": "Pan a mono source directly to binaural stereo with a per-source HRTF (built-in MIT KEMAR) — no ambisonic bus, no order-limited blur. Direction changes crossfade click-free."
    }
   },
   {
    "box": {
     "id": "obj-3",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      96.0,
      60.0,
      22.0
     ],
     "text": "noise~",
     "outlettype": [
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-4",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      110.0,
      98.0,
      120.0,
      20.0
     ],
     "text": "(mono source)"
    }
   },
   {
    "box": {
     "id": "obj-5",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      40.0,
      152.0,
      200.0,
      22.0
     ],
     "text": "ambitap.panbin~",
     "outlettype": [
      "signal",
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-6",
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      212.0,
      90.0,
      45.0
     ],
     "text": "ezdac~",
     "outlettype": []
    }
   },
   {
    "box": {
     "id": "obj-7",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      140.0,
      224.0,
      240.0,
      20.0
     ],
     "text": "left / right to your headphones"
    }
   },
   {
    "box": {
     "id": "obj-8",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      96.0,
      200.0,
      20.0
     ],
     "text": "azimuth (radians, 0 = front)"
    }
   },
   {
    "box": {
     "id": "obj-9",
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      480.0,
      114.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-10",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      546.0,
      114.0,
      100.0,
      22.0
     ],
     "text": "azimuth $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-11",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      146.0,
      220.0,
      20.0
     ],
     "text": "elevation (radians, 0 = horizon)"
    }
   },
   {
    "box": {
     "id": "obj-12",
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      480.0,
      164.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-13",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      546.0,
      164.0,
      100.0,
      22.0
     ],
     "text": "elevation $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-14",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      196.0,
      110.0,
      20.0
     ],
     "text": "gain (linear)"
    }
   },
   {
    "box": {
     "id": "obj-15",
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      480.0,
      214.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-16",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      546.0,
      214.0,
      90.0,
      22.0
     ],
     "text": "gain $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-17",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      254.0,
      380.0,
      47.0
     ],
     "text": "Compare with encode~ → binaural~: panbin~ convolves the source with the HRIR reconstructed exactly at its direction (per-source HRTF), so localization is sharper than any order-limited ambisonic pipeline."
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
      "obj-3",
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
      "obj-6",
      1
     ],
     "source": [
      "obj-5",
      1
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
      "obj-13",
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
      "obj-5",
      0
     ],
     "source": [
      "obj-13",
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
      "obj-5",
      0
     ],
     "source": [
      "obj-16",
      0
     ]
    }
   }
  ]
 }
}
