/*
This class represents the alignment grid of the UI. This grid represents the size of the
actual UI's screen. It has adjustable grid lines and scrolls.
*/

class CoordGrid
{
  
  /*
  The location and size of the grid.
  */
  PVector activeRegionLocation, activeRegionSize;
  
  /*
  The scrolling offset of the grid.
  */
  PVector gridOffset;
  
  /*
  The number of gridlines, initialized to 20.
  */
  int gridLines = 20;
  
  
  /*
  Constructs the CoordGrid, with an initial location and size.
  */
  public CoordGrid(float x, float y, float sx, float sy)
  {
    activeRegionLocation = new PVector(x, y);
    activeRegionSize = new PVector(sx, sy);
    gridOffset = new PVector(0, 0);
    
  }
  
  /*
  Changes the resolution of the grid by <amount>.
  */
  public void changeGridResolution(int amount)
  {
    gridLines += amount;
  }
  
  /*
  Sets the grid resolution to <amount>.
  */
  public void setGridResolution(int amount)
  {
	gridLines = amount;
  }
  
  /*
  Changes the scrolling offset by (<changeX>, <changeY>).
  */
  public void changeOffset(float changeX, float changeY)
  {
    gridOffset.x += changeX;
    gridOffset.y += changeY;
  }
  
  /*
  Sets the scrolling offset to (<changeX>, <changeY>).
  */
  public void setOffset(float newX, float newY)
  {
    gridOffset.x = newX;
    gridOffset.y = newY;
  }
  
  /*
  Returns whether the point (<x>, <y>) is within the grid.
  */
  public boolean pointInActiveRegion(float x, float y)
  {
    if(x > activeRegionLocation.x && x < activeRegionLocation.x + activeRegionSize.x)
    {
      if(y > activeRegionLocation.y && y < activeRegionLocation.y + activeRegionSize.y)
      {
        return true;
      }
    }
    return false;
  }
  
  /*
  Renders the coordinate grid.
  */
  public void render()
  {
    // Render grid lines.
    strokeWeight(1.2);
    stroke(255, 30, 30, 64);
    rectMode(CORNERS);
    
    // Create a secondary PVector so we don't corrupt the original one
    // when we modulate so that we can wrap around the lines of the grid.
    PVector renderingOffset = new PVector(gridOffset.x, gridOffset.y);
    
    // If any grid line is further away from an edge than the width between gridlines,
    // wrap around the current rendering offset so we get a new line.
    while(renderingOffset.x > (activeRegionSize.x/gridLines))
    {
      renderingOffset.x -= (activeRegionSize.x/gridLines);
    }
    while(renderingOffset.x < 0)
    {
      renderingOffset.x += (activeRegionSize.x/gridLines);
    }
    while(renderingOffset.y > (activeRegionSize.y/gridLines))
    {
      renderingOffset.y -= (activeRegionSize.y/gridLines);
    }
    while(renderingOffset.y < 0)
    {
      renderingOffset.y += (activeRegionSize.y/gridLines);
    }
    
    // Draw all the grid lines.
    for(int i = 0; i < gridLines; i++)
    {
      line(activeRegionLocation.x + renderingOffset.x + (activeRegionSize.x/gridLines)*i, activeRegionLocation.y,
          activeRegionLocation.x + renderingOffset.x + (activeRegionSize.x/gridLines)*i, activeRegionLocation.y+activeRegionSize.y);
      line(activeRegionLocation.x, activeRegionLocation.y + renderingOffset.y + (activeRegionSize.y/gridLines)*i,
          activeRegionLocation.x+activeRegionSize.x, activeRegionLocation.y + renderingOffset.y + (activeRegionSize.y/gridLines)*i);
    }
  
    // Draw grid border.
    stroke(255, 30, 30, 255);
    noFill();
    rect(activeRegionLocation.x, activeRegionLocation.y, activeRegionSize.x, activeRegionSize.y);
  }
}
