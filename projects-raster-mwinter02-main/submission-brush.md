# Project 1: Brush

Please fill this out for Brush only. The project handout can be found [here](https://cs1230.graphics/projects/raster/1).

## Output Comparison
This is how you will demonstrate the functionality of your project.

For each of the rows below, follow the instructions to record a video. Please reference the [Screen Recording Guide](https://cs1230.graphics/docs/screen-recording/) for machine-specific guidance on how to do this.

Once you've recorded everything, navigate to this file in Github, click edit, and either select or drag-and-drop each of your videos into the correct location. This will upload them to GitHub (but not include them in the repo) and automatically embed them into this Markdown file by providing a link. Make sure to double-check that they all show up properly in the preview.

> Note: you can ignore the `student_outputs` folder in the repo for now. You will be using it in the next project, Filter.

We're **not** looking for your video to **exactly** match the expected output (i.e. draw the exact same shape). Just make sure to follow the instructions and verify that the recording fully demonstrates the functionality of that part of your project.

### Constant Brush
**Instructions:** Draw with the constant brush.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/241d99b5-947a-407c-b454-410534520aad

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/9fe961c2-3432-4ea0-bd92-67e4e2c4b82b


<!---
Paste your output on top of this comment!
-->



### Linear Brush
**Instructions:** Draw with the linear brush.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/9706fa04-7710-441f-b292-ab010e04dec6

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/e440bd31-b800-4f6a-b5cb-9f9462a4c103


<!---
Paste your output on top of this comment!
-->



### Quadratic Brush
**Instructions:** Draw with the quadratic brush.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/c5df5c09-bfe0-4c05-a56e-14609772d675

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/826b55f4-55f8-4875-95fb-5870739ef62b


<!---
Paste your output on top of this comment!
-->



### Smudge Brush
**Instructions:** Draw some colors on the canvas and use the smudge brush to smear them together.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/26440b63-2d1c-43fd-95f2-55b74ad3bbed

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/3e45606e-d962-44bd-8b07-409f5307388c


<!---
Paste your output on top of this comment!
-->



### Smudge Brush Change in Alpha
**Instructions:** Draw some colors on the canvas. Use the smudge brush with varying alpha levels (use at least three) and demonstrate that the brush still works the same way each time.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/0b49c7d0-47ca-46d0-af72-48b831dfe7ea

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/5be32fb4-54d7-4592-b5c3-b4b1b3202c06


<!---
Paste your output on top of this comment!
-->



### Radius
**Instructions:** Use any brush with at least 3 different values for the radius.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/6f619df6-80cd-4849-8831-6a5aade2a517

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/256a6207-7ed4-458b-b2bb-ec3eb57deab4


<!---
Paste your output on top of this comment!
-->



### Color
**Instructions:** Use any brush to draw red (255, 0, 0), green (0, 255, 0), and blue (0, 0, 255).

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/fd9578ca-e0af-433e-ac9e-b27db2ceebc9

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/b774c75c-e03a-490a-816a-a0a8300e05dc


<!---
Paste your output on top of this comment!
-->



### Canvas Edge Behavior
**Instructions:** With any brush, click and draw on the canvas in a place where the mask intersects with the edge. Then, start drawing anywhere on the canvas and drag your mouse off of the edge.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/f5344248-fa5f-4c33-b6df-ff0a45011c7a

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/3c25aacf-1e67-4e73-bd71-2e6ca819278b


<!---
Paste your output on top of this comment!
-->



### Alpha
**Instructions:** With the constant brush, draw a single dot of red (255, 0, 0) with an alpha of 255. Then, draw over it with a single dot of blue (0, 0, 255) with an alpha of 100. You should get a purpleish color.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/b13d312d-d6d4-4375-aeaa-96174065443b

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/747024ee-2ba3-45c3-be24-a1f6d2c2181b


<!---
Paste your output on top of this comment!
-->



### Alpha of Zero
**Instructions:** Choose any brush and demonstrate that it will not draw if the alpha value is zero.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/8e48777e-8196-401e-9af6-871abe712146

#### Your Output


https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/f58a841d-79c8-4eff-9294-560032448884


<!---
Paste your output on top of this comment!
-->



## Design Choices

Created helper methods such as:
blendColor which calculates the resulting color from a given color and the canvas.
getMaskAlpha which calculates the alpha value based on radius and mask type

No seperate brush class
added a vector<RGBA> for smudge overlay which updates on each mouseDragged event
used a vector<int> for the indexes the mask of a given brush size covers for low processing time.

## Collaboration/References
High level idea collaboration re smudging with @avonbism

## Known Bugs
none so far

## Extra Credit


Fix alpha blending: shown below
kept track of visited pixels using a vector<int> which resets on mouseUp event

https://github.com/BrownCSCI1230/projects-raster-mwinter02/assets/90807019/1ceae39e-42df-458b-9b44-155ccd6c2885




