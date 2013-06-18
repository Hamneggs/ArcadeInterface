/*
This class represents one Tile within the actual arcade interface.
It is essentially a node within a map, with four linking points so
the grid of tiles can be traversed with up, down, left or right movements.

It features three images: an image that is displayed when the Tile is
selected, one when the Tile is not, and another that takes place of the
background when the Tile is selected.

It also stores several parameters about the game it represents:
The name of the game, a (pointless as of now)description, and
the command used to run the game(which is called through system() in the
actual UI.)

It also has a unique ID which is used in reconstructing the neighbor
relationships when loading the Tiles from XML.

Each Tile contains a reference to a Frame image that frames the image content
of the Tile.
*/
class Tile{
  
  /*
  The Tile's location, in terms of pixels. When exported, this is divided by the number
  of pixels in the editor's 'active' window at 100% zoom.
  */
  public PVector location;
  
  /*
  The offset of this Tile in the editor from it's actual location.
  */
  public PVector offset;
  
  /*
  The standard tile w*h ratio.
  */
  public PVector ratio = new PVector(4, 3);
  
  /*
  The size of the Tile in terms of pixels when inactive. This too is divided by the size of
  the editor's 'active' region.
  */
  public PVector inactiveSize = new PVector(80, 60);
  
  /*
  The size of this Tile in terms of pixels when active. This too is divided by the size of
  the editor's 'active' region.
  */
  public PVector activeSize = new PVector(120, 80);
  
  /*
  The north neighbor binding location for arrows, relative to location.
  */
  public final PVector northBinding = new PVector(0, -inactiveSize.y/2);
  
  /*
  The south neighbor binding location for arrows, relative to location.
  */
  public final PVector southBinding = new PVector(0, inactiveSize.y/2);
  
  /*
  The east neighbor binding location for arrows, relative to location.
  */
  public final PVector eastBinding = new PVector(inactiveSize.x/2, 0);
  
  /*
  The east neighbor binding location for arrows, relative to location.
  */
  public final PVector westBinding = new PVector(-inactiveSize.x/2, 0);
  
  /*
  Whether or not this tile is active.
  */
  public boolean isActive;
  
  /*
  Whether or not this Tile has a frame.
  */
  public boolean hasFrame;
  
  /*
  Whether or not this Tile is first, meaning that when the UI starts up, it snaps
  focus to this tile.
  */
  public boolean isFirst;
  
  /*
  The size of the neighbor line binding points.
  */
  public final float bindingPointRadius = 4;
  
  /*
  The internal color of an imageless Tile.
  */
  public final color internal = color(200, 100, 200);
  
  /*
  Text size for ID.
  */
  public final int textSize = 12;
  
  /*
  The external (stroke) color of an imageless Tile.
  */
  public final color external = color(255, 255, 255);
  
  /*
  The active and inactive images of the tile.
  */
  public PImage active = null, inactive = null, bg = null;
  
  /*
  The frame of this Tile.
  */
  public PImage frame;
  
  /*
  References to neighboring tiles.
  */
  public Tile north = null, south = null, east = null, west = null;

  /*
  The command that will be run when this tile is selected.
  */
  String command;
  
  /*
  The name of the game.
  */
  String name;
  
  /*
  The description of the game.
  */
  String description;
  
  /*
  The path to the active screenshot.
  */
  String activePath;
  
  /*
  The path to the inactive screenshot.
  */
  String inactivePath;
  
  /*
  The path to the background screenshot.
  */
  String bgPath;
  
  /*
  The ID of this tile so we can export neighbor relationships easily.
  */
  int id;
  
  /*
  Whether or not this Tile is selectable.
  */
  boolean selectable;
  
  /*
  An empty constructor, since initialization should take place after object creation.
  */
  public Tile()
  {
    isActive = false;
    id = 0;
    name = "<no name specified>";
    description = "<no description specified>";
    command = "<no command specified>";
    activePath = "<no path specified>";
    inactivePath = "<no path specified>";
    bgPath = "<no path specified>";
    location = new PVector(0, 0);
    offset = new PVector(0, 0);
    isFirst = false;
    hasFrame = true;
    selectable = true;
  }
  
  /*
  Initializes the location of this Tile.
  */
  public void initLocation(float x, float y)
  {
    location = new PVector(x, y);
    offset = new PVector(0, 0);
  }
  
  /*
  Initialiizes the frame image.
  */
  public void initFrame(PImage frame)
  {
    this.frame = frame;
  }
  
  /*
  Changes the location of this Tile.
  */
  public void changeLocation(float changeX, float changeY)
  {
    location.add(changeX, changeY, 0);
  }
  
  /*
  Initializes active screenshot image by loading it from the given path.
  */
  public void initActiveImage(String activePath)
  {
    this.activePath = activePath;
    try{
      active = loadImage(activePath);
    }
    catch(Exception e)
    {
      println("could not load active screenshot image.");
    }
  }
  
  /*
  Inialize the inactive screenshot image by loading it from the given path.
  */
  public void initInactiveImage(String inactivePath)
  {
    
    this.inactivePath = inactivePath;
    try
    {
      inactive = loadImage(inactivePath);
    }
    catch(Exception e)
    {
      println("Could not load inactive screenshot image.");
    }
  }
  
  /*
  Inialize the background screenshot image by loading it from the given path.
  */
  public void initBGImage(String bgPath)
  {
    
    this.bgPath = bgPath;
    try
    {
      bg = loadImage(bgPath);
    }
    catch(Exception e)
    {
      println("Could not load background screenshot image.");
    }
  }
  
  /*
  Initializes other data.
  */
  public void initInfo(String name, String description, String command)
  {
    this.name = name;
    this.description = description;
    this.command = command;
  }
  
  /*
  Initializes the ever important ID.
  */
  public void initID(int id)
  {
	  this.id = id;
  }
  
  /*
  Sets the selectablity boolean.
  */
  public void setSelectablity(boolean selectable)
  {
    this.selectable = selectable;
  }
  
  /*
  Returns the selectablity of this Tile.
  */
  public boolean isSelectable()
  {
    return selectable;  
  }
  
  /*
  Initializes the active and inactive sizes of this Tile.
  */
  public void initSize(float activeSX, float activeSY, float inactiveSX, float inactiveSY)
  {
	this.activeSize.set(activeSX, activeSY, 0);
	this.inactiveSize.set(inactiveSX, inactiveSY, 0);
  }
  
  /*
  Initializes the neighbors of this Tile.
  */
  public void initNeighbors(Tile north, Tile south, Tile east, Tile west)
  {
	 this.north = north;
	 this.south = south;
	 this.east = east; 
	 this.west = west;
  }  
  
  /*
  Renders the base of the tile as a magenta rectangle surrounded by the Tile Frame.
  */
  public void renderBase()
  {
    stroke(external);
    fill(internal);
    
    rectMode(CENTER);
    
    if(!isActive)
    {
      rect(location.x+offset.x, location.y+offset.y, inactiveSize.x, inactiveSize.y);
      if(hasFrame)
      {
        imageMode(CENTER);
        image(frame,location.x+offset.x+(inactiveSize.x/25), location.y+offset.y+(inactiveSize.y/25), inactiveSize.x+(inactiveSize.x/5), inactiveSize.y+(inactiveSize.y/5));
      }
    }
    else
    {
      rect(location.x+offset.x, location.y+offset.y, activeSize.x, activeSize.y);
      if(hasFrame)
      {
        imageMode(CENTER);
        image(frame,location.x+offset.x+(activeSize.x/25), location.y+offset.y+(activeSize.y/25), activeSize.x+(activeSize.x/5), activeSize.y+(activeSize.y/5));
      }
    }
  }
  
  /*
  Renders the docks of the neighbor points.
  */
  public void renderBindingPoints()
  {
    noFill();
    stroke(external);
    ellipseMode(CENTER);
    ellipseMode(RADIUS);
    ellipse(eastBinding.x+location.x+offset.x, eastBinding.y+location.y+offset.y, bindingPointRadius, bindingPointRadius);
    ellipse(westBinding.x+location.x+offset.x, westBinding.y+location.y+offset.y, bindingPointRadius, bindingPointRadius);
    ellipse(southBinding.x+location.x+offset.x, southBinding.y+location.y+offset.y, bindingPointRadius, bindingPointRadius);
    ellipse(northBinding.x+location.x+offset.x, northBinding.y+location.y+offset.y, bindingPointRadius, bindingPointRadius);
  }

  /*
  Renders the ID number of this tile above it.
  */
  public void renderID()
  {
    
    textSize(textSize);
    stroke(0, 0, 0);
    fill(255, 255, 255);
    text(id, location.x+offset.x-(inactiveSize.x/2), location.y+offset.y-(inactiveSize.y/2));
  }
  
  /*
  Renders the active and inactive images of this Tile if they are present.
  If either is not present, the tile is darkened.
  */
  public void renderImages()
  {
    if(isActive)
    {
      if(active == null)
      {
        rectMode(CENTER);
        fill(0, 0, 0, 64);
        rect(location.x+offset.x, location.y+offset.y, activeSize.x, activeSize.y);
      }
      else
      {
        imageMode(CENTER);
        image(active, location.x+offset.x, location.y+offset.y, activeSize.x, activeSize.y);
      }
    }
    else
    {
      if(inactive == null)
      {
        rectMode(CENTER);
        fill(0, 0, 0, 64);
        rect(location.x+offset.x, location.y+offset.y, inactiveSize.x, inactiveSize.y);
      }
      else
      {
        imageMode(CENTER);
        image(inactive, location.x+offset.x, location.y+offset.y, inactiveSize.x, inactiveSize.y);
      }
    }
      
  }
  
  /*
  Returns whether or not the tile is active.
  */
  public boolean isActive()
  {
    return isActive;
  }
  
  /*
  Sets the active status of the tile/
  */
  public void setActive(boolean active)
  {
    isActive = active;
  }
  
  /*
  Returns whether or not this Tile is first.
  */
  public boolean isFirst()
  {
    return isFirst;
  }
  
  /*
  Sets the first status of this tile.
  */
  public void setFirst(boolean first)
  {
    isFirst = first;
  }
  
  /*
  Returns whether or not this tile has a frame.
  */
  public boolean hasFrame()
  {
    return hasFrame;
  }
  
  /*
  Sets whether or not this tile has a frame.
  */
  public void setFrame(boolean hasFrame)
  {
    this.hasFrame = hasFrame;
  }
  
  /*
  Returns whether or not the given point is above this Tile.
  */
  public boolean pointIsOver(PVector point)
  {
    if(isActive)
      return (point.x > location.x+offset.x-(activeSize.x/2)) && (point.x < location.x+(activeSize.x/2)+offset.x) &&
      (point.y > location.y+offset.y-(activeSize.y/2)) && (point.y < location.y+(activeSize.y/2)+offset.y);
    else
      return (point.x > location.x+offset.x-(inactiveSize.x/2)) && (point.x < location.x+(inactiveSize.x/2)+offset.x) && 
      (point.y > location.y+offset.y-(inactiveSize.y/2) ) && (point.y < location.y+(inactiveSize.y/2)+offset.y); 
  }
  
  /*
  Exports this game as a child in a greater XML document.
  format:
  <fullConfig>
    <game name="Name of Game" description="Description of game." active_path="active screenshot path" inactive_path="inactive screenshot path" north_id="-1" south_id="-1" east_id="-1" west_id="-1" x="0.12" y="0.15" sx_inactive="0.08" sy_inactive="0.66666667" sx_active="0.08" sy_active="0.66666667" command="commandToRunGame" has_frame="1" isFirst="0"></game>
  </fullConfig>

  
  */
  public void exportXML(XML fullConfig, UI ui)
  {
    // Create a child node in the full config for this game.
    XML thisGame = fullConfig.addChild("game");
    
    // Embed the name.
    thisGame.setString("name", this.name);
    
    // Embed the description.
    thisGame.setString("description", this.description);
    
    // Embed the screenshot paths.
    thisGame.setString("active_path", this.activePath);
    thisGame.setString("inactive_path", this.inactivePath);
    thisGame.setString("bg_path", this.bgPath);

    // Embed this Tile's ID.
    thisGame.setInt("id", this.id);
	
    // Embed the neighbor IDs.
    if( north == null) thisGame.setInt("north_id", -1);
    else thisGame.setInt("north_id", this.north.id);
    
    if( south == null) thisGame.setInt("south_id", -1);
    else thisGame.setInt("south_id", this.south.id);
    
    if( east == null) thisGame.setInt("east_id", -1);
    else thisGame.setInt("east_id", this.east.id);
    
    if( west == null) thisGame.setInt("west_id", -1);
    else thisGame.setInt("west_id", this.west.id);
    
    // Embed the location.
    thisGame.setFloat("x", ((this.location.x-ui.coordGrid.activeRegionLocation.x) / ui.coordGrid.activeRegionSize.x) );
    thisGame.setFloat("y", ((this.location.y-ui.coordGrid.activeRegionLocation.y) / ui.coordGrid.activeRegionSize.y) );
    
    // Embed the active size of the tile.
    thisGame.setFloat("sx_active", (this.activeSize.x/ ui.coordGrid.activeRegionSize.x) );
    thisGame.setFloat("sy_active", (this.activeSize.y/ ui.coordGrid.activeRegionSize.y) );
    
    // Embed the inactive size of the tile.
    thisGame.setFloat("sx_inactive", (this.inactiveSize.x/ ui.coordGrid.activeRegionSize.x));
    thisGame.setFloat("sy_inactive", (this.inactiveSize.y/ ui.coordGrid.activeRegionSize.y) );
    
    // Finally we embed the command.
    thisGame.setString("command", this.command);
    
    // Oh but wait, there's more! We embed the frame and first booleans as well, plus the selectablity boolean.
    if(hasFrame)thisGame.setInt("has_frame", 1);
    else thisGame.setInt("has_frame", 0);
    
    if(isFirst)thisGame.setInt("is_first", 1);
    else thisGame.setInt("is_first", 0);
    
    if(selectable)thisGame.setInt("selectable", 1);
    else thisGame.setInt("selectable", 0);
  }
  
}
 
 
 
 
 
 
 
 
 
 
 
 
