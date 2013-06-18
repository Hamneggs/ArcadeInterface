/*
This class defines the image preview that is seen in the bottom 
corner of the screen.
It displays the three iamges of a Tile when rendered.
If a Tile does not have an image, or the Tile is null, a
placeholder image is drawn instead.
Note that all three images are displayed horizontally in the 
area defined when constructed.
*/
class ImagePreview
{
  /*
  We create an image that is just static for images that Tiles lack.
  */
  PImage nap;
  
  /*
  The location and size of the preview.
  */
  PVector location, size;
  
  /*
  The gap between images.
  */
  int gap;
  
  /*
  Construct a new Image Preview, adding appropriate 
  ControlP5 elements along the way.
  */
  public ImagePreview(ControlP5 cp5, float locX, float locY, float sx, float sy, int gap)
  {
    // Store the location and size.
    location = new PVector(locX, locY);
    size = new PVector(sx, sy);
    
    // Store the distance between images.
    this.gap = gap;
    
    // Create the placeholdet statiic image.
    nap = createImage((int)(sx/3)-(gap/2), (int)sy, RGB);
    nap.loadPixels();
    for(int i = 0; i< nap.pixels.length; i++)
    {
      nap.pixels[i] = color(random(255));
    }
    
    // Add the CP5 Label elements.
    cp5.addTextlabel("previewActive")
      .setText("\"Active\" Image preview")
      .setPosition(location.x, location.y-10)
      ;
      
    cp5.addTextlabel("previewInactive")
      .setText("\"Inactive\" Image preview")
      .setPosition(location.x+(size.x*(1.0/3.0)), location.y-10)
      ;
      
    cp5.addTextlabel("previewBG")
      .setText("\"Background\" Image preview")
      .setPosition(location.x+(size.x*(2.0/3.0)), location.y-10)
      ;
  }
  
  /*
  Renders the image preview based on a given Tile.
  */
  public void render(Tile tile)
  {
    imageMode(CORNER);
    if(tile == null)
    {
      image(nap, location.x, location.y, (size.x*(1.0/3.0))-gap, size.y);
      image(nap, location.x+(size.x*(1.0/3.0)), location.y, (size.x*(1.0/3.0))-(gap/2.0), size.y);
      image(nap, location.x+(size.x*(2.0/3.0))+(gap/2.0), location.y, (size.x*(1.0/3.0)), size.y);
    }
    else
    {
      if(tile.active == null) image(nap, location.x, location.y, (size.x*(1.0/3.0))-gap, size.y);
      else image(tile.active, location.x, location.y, (size.x*(1.0/3.0))-gap, size.y);
      
      if(tile.inactive == null) image(nap, location.x+(size.x*(1.0/3.0)), location.y, (size.x*(1.0/3.0))-(gap/2.0), size.y);
      else image(tile.inactive, location.x+(size.x*(1.0/3.0)), location.y, (size.x*(1.0/3.0))-(gap/2.0), size.y);
      
      if(tile.bg == null) image(nap, location.x+(size.x*(2.0/3.0))+(gap/2.0), location.y, (size.x*(1.0/3.0)), size.y);
      else image(tile.bg, location.x+(size.x*(2.0/3.0))+(gap/2.0), location.y, (size.x*(1.0/3.0)), size.y);
    }
  }
}
