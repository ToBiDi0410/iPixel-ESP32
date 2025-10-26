# GFX
# ⚠️⚠️⚠️ UNSTABLE
This is currently only work-in-progress with only few elements beeing available.

# Example
```json
{
  "type": "GFXGroupElement",
  "x": 0,
  "y": 0,
  "width": 32,
  "height": 32,
  "zIndex": 0,
  "children": [
    {
      "type": "GFXElement",
      "x": 0,
      "y": 0,
      "width": 16,
      "height": 16,
      "zIndex": 0,
      "modifiers": [
        { "type": "GFXBackgroundColorModifier", "r": 255, "g": 0, "b": 0, "a": 255 }
      ]
    },
    {
      "type": "GFXElement",
      "x": 16,
      "y": 0,
      "width": 16,
      "height": 16,
      "zIndex": 0,
      "modifiers": [
        { "type": "GFXBackgroundColorModifier", "r": 0, "g": 255, "b": 0, "a": 255 }
      ]
    },
    {
      "type": "GFXElement",
      "x": 0,
      "y": 16,
      "width": 16,
      "height": 16,
      "zIndex": 0,
      "modifiers": [
        { "type": "GFXBackgroundColorModifier", "r": 0, "g": 0, "b": 255, "a": 255 }
      ]
    },
    {
      "type": "GFXTextElement",
      "x": 16,
      "y": 16,
      "width": 16,
      "height": 16,
      "zIndex": 0,
      "text": "Hi",
      "r": 255,
      "g": 128,
      "b": 32,
      "modifiers": [
        { "type": "GFXBackgroundColorModifier", "r": 255, "g": 255, "b": 255, "a": 255 } /* does not work currently */
      ]
    }
  ]
}
```

# Sending
To send this view to the display call the display's gfxJSON endpoint with this json for `?root=` parameter  
(e.g. `http://123.456.789.12/device/12:34:56:78:90:12/gfxJSON?root=%7B%22type%22%253A%22GFXGroupElement%22%252C%22x%22%253A0%252C%22y%22%253A0%252C%22width%22%253A32%252C%22...`)  
⚠️ You must encode the JSON as URL Parameter (e.g. via Javascript encodeURIComponent)