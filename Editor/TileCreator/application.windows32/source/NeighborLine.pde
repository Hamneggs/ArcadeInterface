/*
This class represents the line drawn between neighbors.
It consists of a line drawn between neighbor points on two
neighbor tiles, with a dot on the receiving Tile.
*/
class NeighborLine
{
  
  /*
  The color of the line.
  */
  final color lineColor = color(20, 255, 240, 128);
  
  /*
  The thickness of the line.
  */
  final float lineThickness = 1.2;
  
  /*
  The radius of the endpoint.
  */
  final float endpointRadius = 4;
  
  /*
  Renders the line to the screen.
  */
  public void render(PVector start, PVector finish)
  {
    // Draw the line itself.
    stroke(lineColor);
    line(start.x, start.y, finish.x, finish.y);
    
    // Draw the endpoint.
    noStroke();
    fill(red(lineColor), green(lineColor), blue(lineColor), 255);
    ellipseMode(CENTER);
    ellipseMode(RADIUS);
    ellipse(finish.x, finish.y, endpointRadius, endpointRadius);
  }
}
