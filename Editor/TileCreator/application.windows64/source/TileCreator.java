import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 
import javax.swing.JFileChooser; 
import javax.swing.UIManager; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class TileCreator extends PApplet {





UI ui;
ControlP5 cp5;

public void setup()
{
  
  cp5 = new ControlP5(this);
  
  ui = new UI(cp5);
  
  size(1280, 768);
}

public void draw()
{
  background(0, 0, 0);
  ui.render();
  frame.setTitle(String.format("Tile Creator by Gerard Geer"));
}

public void keyPressed()
{
  if(key == 'x')
  {
    XML master = createXML("arcade_config");
    ui.grid.exportTilesXML(master, ui);
    println(master);
  }
}

public void controlEvent(ControlEvent theEvent)
{
  ui.controlEvent(theEvent, this);
}
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
    strokeWeight(1.2f);
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
      .setPosition(location.x+(size.x*(1.0f/3.0f)), location.y-10)
      ;
      
    cp5.addTextlabel("previewBG")
      .setText("\"Background\" Image preview")
      .setPosition(location.x+(size.x*(2.0f/3.0f)), location.y-10)
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
      image(nap, location.x, location.y, (size.x*(1.0f/3.0f))-gap, size.y);
      image(nap, location.x+(size.x*(1.0f/3.0f)), location.y, (size.x*(1.0f/3.0f))-(gap/2.0f), size.y);
      image(nap, location.x+(size.x*(2.0f/3.0f))+(gap/2.0f), location.y, (size.x*(1.0f/3.0f)), size.y);
    }
    else
    {
      if(tile.active == null) image(nap, location.x, location.y, (size.x*(1.0f/3.0f))-gap, size.y);
      else image(tile.active, location.x, location.y, (size.x*(1.0f/3.0f))-gap, size.y);
      
      if(tile.inactive == null) image(nap, location.x+(size.x*(1.0f/3.0f)), location.y, (size.x*(1.0f/3.0f))-(gap/2.0f), size.y);
      else image(tile.inactive, location.x+(size.x*(1.0f/3.0f)), location.y, (size.x*(1.0f/3.0f))-(gap/2.0f), size.y);
      
      if(tile.bg == null) image(nap, location.x+(size.x*(2.0f/3.0f))+(gap/2.0f), location.y, (size.x*(1.0f/3.0f)), size.y);
      else image(tile.bg, location.x+(size.x*(2.0f/3.0f))+(gap/2.0f), location.y, (size.x*(1.0f/3.0f)), size.y);
    }
  }
}
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
  final int lineColor = color(20, 255, 240, 128);
  
  /*
  The thickness of the line.
  */
  final float lineThickness = 1.2f;
  
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
  public final int internal = color(200, 100, 200);
  
  /*
  Text size for ID.
  */
  public final int textSize = 12;
  
  /*
  The external (stroke) color of an imageless Tile.
  */
  public final int external = color(255, 255, 255);
  
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
 
 
 
 
 
 
 
 
 
 
 
 
/*
This class represents the grid (or more realistically the map) of Tiles.
*/
class TileGrid
{
  
  /*
  The collection we use to store all the tiles.
  */
  ArrayList<Tile> tiles;
  
  /*
  The view offset of the grid from its true location.
  */
  public PVector offset;
  
  /*
  The image that is used for the frame of the Tiles.
  */
  public PImage frame;
  
  /*
  Whether or not to render the neighbor relation arrows.
  */
  public boolean renderRelations;
  
  /*
  Whether or not to render the ID flags.
  */
  public boolean renderIDs;
  
  /*
  Whether or not to render images.
  */
  public boolean renderImages;
  
  /*
  The NeighborLine instance used to draw the neighbor relationships between tiles.
  Only one is needed since its values can be updated for each Tile.
  */
  NeighborLine line;
  
  /*
  A value to keep track of the ID to be assigned to the next added Tile.
  */
  int currentID;
  
  /*
  Constructs a new Tile Grid.
  */
  public TileGrid()
  {
    tiles = new ArrayList<Tile>();
    line = new NeighborLine();
    renderRelations = true;
    renderIDs = true;
    renderImages = false;
    currentID = 0;
    offset = new PVector(0, 0);
    frame = loadImage("editor/frame.png");
    
  }
  
  /*
  Adds any number of Tiles.
  */
  public void addTile(Tile ... tilesToAdd)
  {
    for(int i = 0; i < tilesToAdd.length; i++){
      tilesToAdd[i].id = currentID;
      tilesToAdd[i].offset = new PVector(offset.x, offset.y);
      tilesToAdd[i].initFrame(frame);
      currentID ++;
      tiles.add(tilesToAdd[i]);
    }
  }
  
  /*
  Adds an array of Tiles. NOTICE THAT THIS DOES NOT ASSIGN NEW IDS.
  This is used when loading Tiles from file, when they already have IDs.
  */
  public void addTiles(Tile[] tilesToAdd)
  {
    for(int i = 0; i < tilesToAdd.length; i++){
      tilesToAdd[i].offset = new PVector(offset.x, offset.y);
      tilesToAdd[i].initFrame(frame);
      currentID ++;
      tiles.add(tilesToAdd[i]);
    }
  }
  
  /*
  Renders the Tile bases, the Tile images if (needed), the neighbor relations
  (if needed), and the IDs of the Tiles (if needed).
  */
  public void render()
  {
    for(Tile tile : tiles)
    {
      tile.renderBase();
    }
    if(renderImages)
    {
      for(Tile tile: tiles)
      {
        tile.renderImages();
      }
    }
    if(renderRelations)
    {
      for(Tile tile : tiles)
      {
        tile.renderBindingPoints();
        if(tile.north != null)
        {
          line.render(PVector.add(PVector.add(tile.location, tile.offset), tile.northBinding), PVector.add(PVector.add(tile.north.location, tile.north.offset), tile.north.southBinding));
        }
        if(tile.south != null)
        {
          line.render(PVector.add(PVector.add(tile.location, tile.offset), tile.southBinding), PVector.add(PVector.add(tile.south.location, tile.south.offset), tile.south.northBinding));
        }
        if(tile.east != null)
        {
          line.render(PVector.add(PVector.add(tile.location, tile.offset), tile.eastBinding), PVector.add(PVector.add(tile.east.location, tile.east.offset), tile.east.westBinding));
        }
        if(tile.west != null)
        {
          line.render(PVector.add(PVector.add(tile.location, tile.offset), tile.westBinding), PVector.add(PVector.add(tile.west.location, tile.west.offset), tile.west.eastBinding));
        }
      }
    }
    if(renderIDs)
    {
      for(Tile tile : tiles)
      {
        tile.renderID();
      }
    }
  }
  
  /*
  Returns a Tile based on its ID.
  */
  public Tile getTile(int id)
  {
    for(Tile tile: tiles)
    {
      if(tile.id == id)
      {
        return tile;
      }
    }
    return null;
  }
  
  /*
  Returns a the first Tile that covers the given location,
  null if none.
  */
  public Tile getTileAtLocation(PVector point)
  {
    for(Tile tile: tiles)
    {
      if(tile.pointIsOver(point))
      {
        return tile;
      }
    }
    return null;
  }
  
  /*
  Deletes a Tile based on its ID.
  */
  public void deleteTile(int id)
  {
    for(int i = 0; i < tiles.size(); i++)
    {
      if(tiles.get(i).id == id)
      {
        tiles.remove(i);
        if(tiles.size() == 0)
        {
          currentID = 0;
        }
        return;
      }
    }
  }
  
  /*
  Removes all Tiles.
  */
  public void clearGrid()
  {
    tiles = new ArrayList<Tile>();
    currentID = 0;
  }
  
  /*
  Changes the lcoation of every Tile by the same amount.
  */
  public void changeTileLocations(float changeX, float changeY)
  {
    for(Tile tile : tiles)
    {
      tile.location.add(changeX, changeY, 0);
    }
  }
  
  /*
  Sets the active size of every Tile.
  */
  public void setTileActiveSize(float sizeX, float sizeY)
  {
    for(Tile tile : tiles)
    {
      tile.activeSize.x = sizeX * tile.ratio.x;
      tile.activeSize.y = sizeX * tile.ratio.y;
    }
  }
  
  /*
  Sets the inactive size of every Tile.
  */
  public void setTileInactiveSize(float sizeX, float sizeY)
  {
    for(Tile tile : tiles)
    {
      tile.inactiveSize.x = sizeX * tile.ratio.x;
      tile.inactiveSize.y = sizeX * tile.ratio.y;
    }
  }
  
  /*
  Changes the offset of each Tile, so that the grid can be
  scrolled.
  */
  public void changeTileOffsets(float changeX, float changeY)
  {
    for(Tile tile : tiles)
    {
      tile.offset.add(changeX, changeY, 0);
    }
  }
  
  /*
  Sets the scrolling offset of the Tiles.
  */
  public void setTileOffsets(float newX, float newY)
  {
    for(Tile tile : tiles)
    {
      tile.offset.set(newX, newY, 0);
    }
  }
  
  /*
  Exports each Tile as a node in the given XML object.
  This requires information from the UI.
  */
  public void exportTilesXML(XML toExportInto, UI ui)
  {
    for(Tile tile : tiles)
    {
      tile.exportXML(toExportInto, ui);
    }
  }
  
  /*
  Sets every Tile to not be active, deselecting all Tiles.
  */
  public void clearActiveTileStatus()
  {
    for(Tile tile : tiles)
    {
      tile.setActive(false);
    }
  }
  
  /*
  Makes sure that no Tile is declared "first", so that
  a new Tile can be declared as such without conflict.
  */
  public void clearFirstStatus()
  {
    for(Tile tile : tiles)
    {
      tile.setFirst(false);
    }
  }
  
  /*
  Returns the number of Tiles in the grid.
  */
  public int getNumTiles()
  {
    return tiles.size();
  }
  
  /*
  Toggles whether or not IDs are rendered.
  */
  public void toggleIDs()
  {
    renderIDs = !renderIDs;
  }
  
  /*
  Toggles whether or not Neighbor relations are rendered.
  */
  public void toggleNeighborRelations()
  {
    renderRelations = !renderRelations;
  }
  
  /*
  Toggles whether or not Tile images are rendered.
  */
  public void toggleImages()
  {
    renderImages = !renderImages;
  }
      
}
public class UI
{

  /*
  The Tile Grid used to store all the Tiles.
  */
  TileGrid grid;
  
  /*
  The coordinate grid that makes up the working plane.
  */
  CoordGrid coordGrid;

  /*
  Whether or not a single item is being dragged.
  */
  boolean movingItem;

  /*
  References to the current and most recent Tiles, for monitoring
  and editing purposes.
  */
  Tile currentTile, mostRecentTile;

  /*
  The ControlP5 bay that contains all the control elements used.
  */
  ControlP5 cp5;

  /*
  The ImagePreview that displays the images of the most recent Tile.
  */
  ImagePreview preview;

  /*
  Textfields for the various text-based properties of a Tile.
  */
  Textfield name, description, command, activePath, inactivePath, bgPath;

  /*
  Dropdown lists for selecting neighbors.
  */
  DropdownList northNeighbor, southNeighbor, eastNeighbor, westNeighbor;

  /*
  Toggles to toggle properties of the editor and of the most recent Tile.
  */
  Toggle images, relations, ids, hasFrame, isSelectable, isFirst;

  /*
  Sliders for the size of the most recent Tile and the grid resolution.
  */
  Slider gridRes, activeSize, inactiveSize;

  /*
  Buttons to perform various functions.
  */
  Button newTile, deleteTile, exportXML, loadXML, deselectAllTiles, exportImagesButton, resetScroll, activePathButton, inactivePathButton, bgPathButton;

  /*
  The file chooser dialog that is used to save and load XML files, and to get the path of various images.
  */
  final JFileChooser fileChooser = new JFileChooser();

  /*
  Text labels to display various editor and most recent Tile info.
  */
  Textlabel currentTileInfo, currentTileLocation, currentOffset;

  /*
  Constructs the UI.
  */
  public UI(ControlP5 cp5)
  {
    coordGrid = new CoordGrid(10, 10, 750, 600);
    grid = new TileGrid();
    movingItem = false;
    currentTile = null;

    initControls(cp5);

    preview = new ImagePreview(cp5, 780, 600, 498, 150, 2);

    try 
    { 
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    } 
    catch (Exception e) 
    { 
      e.printStackTrace();
      println("COULD NOT USE SYSTEM LOOK AND FEEL");
    }
  }

  /*
  Initializes the CP5 controls that the UI needs.
  */
  public void initControls(ControlP5 controlP5)
  {
    cp5 = controlP5;

    deselectAllTiles = cp5.addButton("deselectTiles")
      .setLabel("Deselect All Tiles")
        .setPosition(1050, 9)
          .setSize(99, 19)
            ;

    newTile = cp5.addButton("newTile")
      .setLabel("Create New Tile")
        .setPosition(1050, 30)
          .setSize(99, 19)
            ;

    deleteTile = cp5.addButton("deleteTile")
      .setLabel("Delete Current Tile")
        .setPosition(1151, 30)
          .setSize(99, 19)
            ;

    exportXML = cp5.addButton("exportXML")
      .setLabel("Export as XML")
        .setPosition(1050, 51)
          .setSize(99, 19)
            ;

    loadXML = cp5.addButton("importXML")
      .setLabel("Load XML File")
        .setPosition(1151, 51)
          .setSize(99, 19)
            ;

    resetScroll = cp5.addButton("resetScroll")
      .setLabel("Reset Scroll Offset")
        .setPosition(1151, 9)
          .setSize(99, 19)
            ;
            
    activePathButton = cp5.addButton("exportImagesButton")
      .setPosition(949, 30)
        .setSize(99, 19)
          .setLabel("Export Images")
            ;

    gridRes = cp5.addSlider("gridRes")
      .setPosition(20, 720)
        .setSize(180, 15)
          .setRange(10, 100)
            .setNumberOfTickMarks(19) 
              .setSliderMode(Slider.FLEXIBLE)
                .setLabel("Grid Resolution")
                  .setValue(coordGrid.gridLines);
    ;

    hasFrame = cp5.addToggle("hasFrame")
      .setPosition(760, 9)
        .setSize(20, 20)
          .setValue(true)
            .setLabel("Frame?")
              ;

    isFirst = cp5.addToggle("selectable")
      .setPosition(820, 9)
        .setSize(20, 20)
          .setValue(true)
            .setLabel("Selectable")
              ;
    
    isFirst = cp5.addToggle("isFirst")
      .setPosition(880, 9)
        .setSize(20, 20)
          .setValue(false)
            .setLabel("Initial Tile")
              ;

    activeSize = cp5.addSlider("activeSize")
      .setPosition(275, 650)
        .setSize(360, 15)
          .setRange(1, 50)
            .setNumberOfTickMarks(50)
              .setLabel("Active Tile Size (times 4 : 3)")
                .setValue(coordGrid.gridLines*1.5f);
    ;

    inactiveSize = cp5.addSlider("inactiveSize")
      .setPosition(275, 720)
        .setSize(360, 15)
          .setRange(1, 50)
            .setNumberOfTickMarks(50)
              .setLabel("Inactive Tile Size (times 4 : 3)")
                .setValue(coordGrid.gridLines)
                  ;     

    images = cp5.addToggle("toggleImages")
      .setPosition(20, 620)
        .setSize(10, 10)
            .setLabel("Render Images")
              .setValue(false)
              ;

    relations = cp5.addToggle("toggleRelationships")
      .setPosition(20, 650)
        .setSize(10, 10)
            .setLabel("Render Neighbor Relationships")
              .setValue(true)
              ;
    ids = cp5.addToggle("toggleIDs")
      .setPosition(20, 680)
        .setSize(10, 10)
            .setLabel("Render Tile IDs.")
              .setValue(true)
              ;

    activePathButton = cp5.addButton("activePathButton")
      .setPosition(1255, 205)
        .setSize(15, 15)
          .setLabel("...")
            ;

    inactivePathButton = cp5.addButton("inactivePathButton")
      .setPosition(1255, 245)
        .setSize(15, 15)
          .setLabel("...")
            ;
    bgPathButton = cp5.addButton("bgPathButton")
      .setPosition(1255, 285)
        .setSize(15, 15)
          .setLabel("...")
            ;

    currentOffset = cp5.addTextlabel("curOffset")
      .setText("CURRENT SCROLL OFFSET:\nX: ?\nY: ?")
        .setPosition(760, 60)
          ;

    currentTileLocation = cp5.addTextlabel("curTileLoc")
      .setText("CURRENT TILE LOCATION:\nX: ?\nY: ?")
        .setPosition(760, 100)
          ;

    currentTileInfo = cp5.addTextlabel("curTile")
      .setText("CURRENT TILE INFO:\nNone")
        .setPosition(760, 400)
          ;

    name = cp5.addTextfield("name")
      .setPosition(950, 85)
        .setSize(300, 15)
          .setAutoClear(false)
            .setLabel("Game Name")
              ;

    description = cp5.addTextfield("desc")
      .setPosition(950, 125)
        .setSize(300, 15)
          .setAutoClear(false)
            .setLabel("Game Description (Include newlines)")
              ;

    command = cp5.addTextfield("command")
      .setPosition(950, 165)
        .setSize(300, 15)
          .setAutoClear(false)
            .setLabel("Command (You better format it correctly!)");
    ;

    activePath = cp5.addTextfield("activePath")
      .setPosition(925, 205)
        .setSize(325, 15)
          .setAutoClear(false)
            .setLabel("Path for \"active\" screenshot.")
              ;

    inactivePath = cp5.addTextfield("inactivePath")
      .setPosition(925, 245)
        .setSize(325, 15)
          .setAutoClear(false)
            .setLabel("Path for \"inactive\" screenshot.")
              ;

    bgPath = cp5.addTextfield("bgPath")
      .setPosition(925, 285)
        .setSize(325, 15)
          .setAutoClear(false)
            .setLabel("Path for \"background\" screenshot.")
              ;

    northNeighbor = cp5.addDropdownList("north")
      .setPosition(880, 350)
        .setSize(80, 120)
          .setItemHeight(15)
            .setLabel("North Neighbor")
              ;

    southNeighbor = cp5.addDropdownList("south")
      .setPosition(980, 350)
        .setSize(80, 120)
          .setItemHeight(15)
            .setLabel("South Neighbor")
              ;

    eastNeighbor = cp5.addDropdownList("east")
      .setPosition(1080, 350)
        .setSize(80, 120)
          .setItemHeight(15)
            .setLabel("East Neighbor")
              ;

    westNeighbor = cp5.addDropdownList("west")
      .setPosition(1180, 350)
        .setSize(80, 120)
          .setItemHeight(15)
            .setLabel("West Neighbor")
              ;
  }

  /*
  Returns the currently selected Tile.
  */
  public Tile getCurrentTile()
  {
    return currentTile;
  }

  /*
  Renders the UI.
  */
  public void render()
  {  
    // Render grid lines
    coordGrid.render();

    // Render grid
    grid.render();

    handleMouse();

    preview.render(mostRecentTile);
  }

  /* 
  Loads Tiles from XML.
   */
  public void loadTileFile(File file)
  {
    // We load the file only if it ends with .xml.
    if (file.getName().endsWith(".xml")||
      file.getName().endsWith(".XML"))
    {
      // Load the top-level list of nodes.
      XML gameList = loadXML(file.getPath());

      // Print out the raw file.
      println("THIS IS WHAT WE ARE LOADING:\n"+gameList);

      // Get the individual games.
      XML[] games = gameList.getChildren("game");

      // Print out the number of games that we have.
      println("Number of games: "+games.length);

      // Create an array of blank Tiles to load the data into.
      Tile[] loadedTiles = new Tile[games.length];

      // For each game in the XML we initialize a Tile in the array with its data, excluding neighbor relations.
      for (int i = 0; i < games.length; i++)
      {
        println("Loading tile #"+i+"...");

        // First we do a test to see if the current game XML has the proper number of attributes. If it doesn't,
        // then there is a high likelyhood that the file is bad.
        int attCount = games[i].getAttributeCount();
        if (attCount != 20)
        {
          println("Improperly formatted game at index "+i+" (Number of attributes is "+attCount+" and should be 20. Aborting...");
          return;
        }
        // Even though we are using the index of one array for another, they are the same length,
        // and thusly safe.
        loadedTiles[i] = new Tile();

        println("\tLoading Tile #"+i+"'s location...");
        // Load the location.
        loadedTiles[i].initLocation((games[i].getFloat("x")*coordGrid.activeRegionSize.x)+coordGrid.activeRegionLocation.x, (games[i].getFloat("y")*coordGrid.activeRegionSize.y)+coordGrid.activeRegionLocation.y );

        println("\tLoading Tile #"+i+"'s image paths...");
        // Load the images.
        loadedTiles[i].initActiveImage(games[i].getString("active_path"));
        loadedTiles[i].initInactiveImage(games[i].getString("inactive_path"));
        loadedTiles[i].initBGImage(games[i].getString("bg_path"));

        println("\tLoading Tile #"+i+"'s name, desc and command...");
        // Load the rest of the Tile's information (Name, Description, and Run Command).
        loadedTiles[i].initInfo(games[i].getString("name"), 
        games[i].getString("description"), 
        games[i].getString("command"));

        println("\tLoading Tile \"First\", \"selectable\", and \"Frame\" stats");
        if (games[i].getInt("has_frame") == 0) loadedTiles[i].setFrame(false);
        else loadedTiles[i].setFrame(true);
        if (games[i].getInt("selectable") == 0) loadedTiles[i].setSelectablity(false);
        else loadedTiles[i].setFrame(true);
        if (games[i].getInt("is_first") == 1) loadedTiles[i].setFirst(true);
        else loadedTiles[i].setFirst(false);

        println("\tLoading Tile #"+i+"'s ID...");
        // Load the Tile's ID.
        loadedTiles[i].initID(games[i].getInt("id"));

        println("\tLoading Tile #"+i+"'s active and inactive sizes...");
        // Now we load the active and inactive sizes.
        loadedTiles[i].initSize((games[i].getFloat("sx_active")*coordGrid.activeRegionSize.x)+coordGrid.activeRegionLocation.x, 
        (games[i].getFloat("sy_active")*coordGrid.activeRegionSize.y)+coordGrid.activeRegionLocation.y, 
        (games[i].getFloat("sx_inactive")*coordGrid.activeRegionSize.x)+coordGrid.activeRegionLocation.x, 
        (games[i].getFloat("sy_inactive")*coordGrid.activeRegionSize.y)+coordGrid.activeRegionLocation.y);
        println("...Done loading Tile #"+i+".");
      }

      // Now that all tiles are loaded, we load the tiles into a new TileGrid so that we can 
      // bind them to their neighbors. This loading method allows for the preservation of existing
      // ID assignments.
      println("Constructing new Tile Grid...");
      TileGrid newGrid = new TileGrid();
      println("Adding "+loadedTiles.length+" loaded Tiles to Grid...");
      newGrid.addTiles(loadedTiles);
      println(newGrid.getNumTiles()+" Tiles added to Grid.");

      println("Constructing neighbor relationships...");
      // Finally we go through each of the loaded Tiles and bind it to its neighbors, if it has any.
      for (int i = 0; i < loadedTiles.length; i++)
      {
        println("\tSetting neighbor relations for tile ID: "+loadedTiles[i].id+"...");
        loadedTiles[i].initNeighbors(newGrid.getTile(games[i].getInt("north_id")), 
        /*  EMPTY SPACE....       */        newGrid.getTile(games[i].getInt("south_id")), 
        /*                        */        newGrid.getTile(games[i].getInt("east_id")), 
        /*  ....HOLLA AT ME BBZ   */        newGrid.getTile(games[i].getInt("west_id")));
      }
      println("...Done constructing neighbor relationships.");
      // At last we swap the new grid for the old one.
      println("Swapping new and old Grid.");
      grid = newGrid;
      println("...Done loading Tiles!");

      // Finally we load shit to configure the editor. We do this last so that we can set the offset of the tiles properly.
      println("Loading editor data...");
      XML editorData = gameList.getChildren("editor_data")[0]; // We only take the first one.
      println("\tSetting scroll offset...");
      setOffset(editorData.getFloat("x_offset"), editorData.getFloat("y_offset"));
      println("\tSetting grid resolution...");
      coordGrid.setGridResolution(editorData.getInt("grid_lines"));
      gridRes.setValue(coordGrid.gridLines);
      println("...Done loading editor data.");
    }
    else
    {
      println("NOT AN XML FILE YOU FUCKUP.");
    }
  }
  
  /*
  Exports Tile and UI data.
  */
  public void saveTileFile(File file)
  {
    XML tileData = createXML("arcade_config");
    XML editorData = tileData.addChild("editor_data");
    editorData.setFloat("x_offset", coordGrid.gridOffset.x);
    editorData.setFloat("y_offset", coordGrid.gridOffset.y);
    editorData.setInt("grid_lines", coordGrid.gridLines);
    grid.exportTilesXML(tileData, this);
    println(tileData);
    saveXML(tileData, file.getPath());
  }

  /*
  THIS FUNCTION IS IMPORTANT. Saves all Tile images to a chosen
  directory as .pngs with standardized filenames, then updates the
  paths of each Tile to the new filename and parent directory.
  */
  public void exportImages(File path)
  {
    String newPath = getTruncatedPath(path);
    for(int i = 0; i < grid.tiles.size(); i++)
    {
      Tile t = grid.tiles.get(i);
      currentTile = t;
      mostRecentTile = t;
      
      if(t.active!= null)
      {
        //t.active.resize(512, 512);
        t.active.save(path.getPath() + path.separatorChar + "tile_"+t.id+"_active.png");
        println("active image saved to: "+path.getPath() + path.separatorChar + "tile_"+t.id+"_active.png");
        t.activePath = newPath + path.separatorChar + "tile_"+t.id+"_active.png";
      }
      
      if(t.inactive!= null)
      {
        //t.inactive.resize(512, 512);
        t.inactive.save(path.getPath() + path.separatorChar + "tile_"+t.id+"_inactive.png"); 
        println("inactive image saved to: "+path.getPath() + path.separatorChar + "tile_"+t.id+"_inactive.png");
        t.inactivePath = newPath + path.separatorChar + "tile_"+t.id+"_inactive.png";
      }
      
      if(t.bg!=null)
      {
        //t.bg.resize(512, 512);
        t.bg.save(path.getPath() + path.separatorChar + "tile_"+t.id+"_bg.png");
        println("bg  image saved to: "+path.getPath() + path.separatorChar + "tile_"+t.id+"_bg.png");
        t.bgPath = newPath + path.separatorChar + "tile_"+t.id+"_bg.png";
      }
      
      updateTextBoxes();
      updateCurrentTileText();
    }
  
  }
  
  /*
  Returns a String containing the path trimmed down to be only the first directory above.
  */
  public String getTruncatedPath(File path)
  {
    String initialPath = path.getPath();
    int indexOfLastSeparator = initialPath.lastIndexOf(path.separatorChar);
    int indexOfPenultimateSeparator = initialPath.lastIndexOf(path.separatorChar, indexOfLastSeparator);
    String newPath = initialPath.substring(indexOfPenultimateSeparator+1);
    println("OLD PATH: "+initialPath+"\nNEW PATH:"+newPath);
    return newPath;
    
    
  } 

  /*
  Changes the resolution of the grid.
  */
  public void changeGridResolution(int change)
  {
    coordGrid.changeGridResolution(change);
  }

  /*
  Changes the offset of the grid and Tiles for scrolling.
  */
  public void changeOffset(float changeX, float changeY)
  {
    coordGrid.changeOffset(changeX, changeY);
    grid.changeTileOffsets(changeX, changeY);
  }

  /*
  Directly sets the scrolling offset.
  */
  public void setOffset(float newX, float newY)
  {
    coordGrid.setOffset(newX, newY);
    grid.setTileOffsets(newX, newY);
    currentOffset.setText("CURRENT SCROLLING OFFSET:\nX: "+(coordGrid.gridOffset.x/coordGrid.activeRegionSize.x)+"\nY: "+(coordGrid.gridOffset.y/coordGrid.activeRegionSize.y));
  }

  /*
  Updates the text info to reflect the most recent state of the mostRecentTile.
  */
  public void updateCurrentTileText()
  {
    String northID = "?";
    String southID = "?";
    String eastID = "?";
    String westID = "?";
    if (mostRecentTile.north != null) northID = ""+mostRecentTile.north.id;
    if (mostRecentTile.south != null) southID = ""+mostRecentTile.south.id;
    if (mostRecentTile.east != null) eastID = ""+mostRecentTile.east.id;
    if (mostRecentTile.west != null) westID = ""+mostRecentTile.west.id;
    currentTileInfo.setText("CURRENT TILE INFO:\nID: "+mostRecentTile.id+
      "\nNAME: "+mostRecentTile.name+
      "\nDESCRIPTION: "+mostRecentTile.description+
      "\nCOMMAND: "+mostRecentTile.command+
      "\nACTIVE PATH: "+mostRecentTile.activePath+
      "\nINACTIVE PATH: "+mostRecentTile.inactivePath+
      "\nBACKGROUND PATH: "+mostRecentTile.bgPath+
      "\nNORTH NEIGHBOR: "+northID+
      "\nSOUTH NEIGHBOR: "+southID+
      "\nEAST NEIGHBOR: "+eastID+
      "\nWEST NEIGHBOR: "+westID);
    currentTileLocation.setText("CURRENT TILE LOCATION:\nX: "+(mostRecentTile.location.x/coordGrid.activeRegionSize.x)+
      "\nY: "+(mostRecentTile.location.y/coordGrid.activeRegionSize.y));
    activeSize.setValue(mostRecentTile.activeSize.x/mostRecentTile.ratio.x);
    inactiveSize.setValue(mostRecentTile.inactiveSize.x/mostRecentTile.ratio.x);
    println("updatedText");
  }

  /*
  Populates the neighbor lists based on the currentTile.
  */
  public void updateNeighborLists()
  {
    northNeighbor.clear();
    southNeighbor.clear();
    eastNeighbor.clear();
    westNeighbor.clear();

    northNeighbor.addItem("None", -1);
    southNeighbor.addItem("None", -1);
    eastNeighbor.addItem("None", -1);
    westNeighbor.addItem("None", -1);

    Tile tile;
    for (int i = 0; i < grid.tiles.size(); i++)
    {
      tile = grid.tiles.get(i);
      if (tile!=currentTile)
      {
        String tileName = tile.name;
        if (tileName.length() > 12)
        {
          tileName = tileName.substring(0, 9)+"...";
        }
        northNeighbor.addItem(tile.id +" ("+tileName+")", tile.id);
        southNeighbor.addItem(tile.id +" ("+tileName+")", tile.id);
        eastNeighbor.addItem(tile.id +" ("+tileName+")", tile.id);
        westNeighbor.addItem(tile.id +" ("+tileName+")", tile.id);
      }
    }
  }

  /*
  Updates the value stored in the text boxes.
  */
  public void updateTextBoxes()
  {
    if (mostRecentTile!=null)
    {
      name.setText(mostRecentTile.name);
      description.setText(mostRecentTile.description);
      command.setText(mostRecentTile.command);
      activePath.setText(mostRecentTile.activePath);
      inactivePath.setText(mostRecentTile.inactivePath);
      bgPath.setText(mostRecentTile.bgPath);
    }
  }

  /*
  Updates the value of the hasFrame and first buttons
  based on the current Tile.
  */
  public void updateToggles()
  {
    if (currentTile != null)
    {
      hasFrame.setValue(currentTile.hasFrame());
      isFirst.setValue(currentTile.isFirst());
    }
  }

  /*
  Handles mouse activity.
  */
  public void handleMouse()
  {
    if (mousePressed)
    {
      if (coordGrid.pointInActiveRegion(mouseX, mouseY) || movingItem) {
        movingItem = true;
        if (currentTile == null)
        {
          currentTile = grid.getTileAtLocation(new PVector(mouseX, mouseY));
          if (currentTile != null)
          {
            mostRecentTile = currentTile;
            grid.clearActiveTileStatus();
            currentTile.setActive(true);

            updateTextBoxes();
            updateNeighborLists();
            updateToggles();
          }
        }
        if (currentTile == null)
        {
          changeOffset(mouseX-pmouseX, mouseY-pmouseY);
          currentOffset.setText("CURRENT SCROLLING OFFSET:\nX: "+(coordGrid.gridOffset.x/coordGrid.activeRegionSize.x)+"\nY: "+(coordGrid.gridOffset.y/coordGrid.activeRegionSize.y));
        }
        else
        {
          currentTile.changeLocation(mouseX-pmouseX, mouseY-pmouseY);
          updateCurrentTileText();
        }
      }
    }
    else
    {
      movingItem = false;
      currentTile = null;
    }
  }

  /*
  The unholy controlP5 callback function. 
  */
  public void controlEvent(ControlEvent theEvent, PApplet papplet)
  {
    if (theEvent.isController()) {
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Toggle Images.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("toggleImages"))
      {
        println("Toggling tile iamges...");
        grid.toggleImages();
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Toggle IDs.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("toggleIDs"))
      {
        println("Toggling tile IDs...");
        grid.toggleIDs();
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Toggle Relationships.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("toggleRelationships"))
      {
        println("Toggling relationship rendering...");
        grid.toggleNeighborRelations();
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Deselect Tiles.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("deselectTiles"))
      {
        println("Deselecting Tiles...");
        currentTile = null;
        mostRecentTile = null;
        grid.clearActiveTileStatus();
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Create new tile.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("newTile"))
      {
        println("Creating new tile...");
        Tile tileToAdd = new Tile();
        tileToAdd.initLocation(coordGrid.activeRegionLocation.x+coordGrid.activeRegionSize.x/2, coordGrid.activeRegionLocation.x+coordGrid.activeRegionSize.x/2);
        tileToAdd.initInfo(name.getText(), description.getText(), command.getText());
        tileToAdd.initActiveImage(activePath.getText());
        tileToAdd.initInactiveImage(inactivePath.getText());
        tileToAdd.initBGImage(bgPath.getText());
        grid.clearActiveTileStatus();
        currentTile = tileToAdd;
        mostRecentTile = tileToAdd;
        grid.addTile(tileToAdd);
        mostRecentTile.setActive(true);
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Delete CurrentTile
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("deleteTile"))
      {
        println("Deleting Tile...");
        if (mostRecentTile != null)
        {
          if (mostRecentTile.north != null)
          {
            mostRecentTile.north.south = null;
          }
          if (mostRecentTile.south != null)
          {
            mostRecentTile.south.north = null;
          }
          if (mostRecentTile.east != null)
          {
            mostRecentTile.east.west = null;
          }
          if (mostRecentTile.west != null)
          {
            mostRecentTile.west.east = null;
          }

          int mostRecentID = mostRecentTile.id;
          mostRecentTile = null;
          currentTile = null;
          grid.clearActiveTileStatus();
          grid.deleteTile(mostRecentID);
        }
        else
        {
          println("No current tile to delete.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Load XML file
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("importXML"))
      {
        println("Starting file chooser to load XML file...");
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        int returnVal = fileChooser.showOpenDialog(papplet);

        if (returnVal == JFileChooser.APPROVE_OPTION)
        {
          File file = fileChooser.getSelectedFile();
          loadTileFile(file);
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Save XML file
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("exportXML"))
      {
        println("Starting file chooser to save XML file...");
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        int returnVal = fileChooser.showSaveDialog(papplet);

        if (returnVal == JFileChooser.APPROVE_OPTION)
        {
          File file = fileChooser.getSelectedFile();
          saveTileFile(file);
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Export images to a standard directory.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("exportImagesButton"))
      {
        println("Starting file chooser to set an images directory...");
        fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        int returnVal = fileChooser.showSaveDialog(papplet);

        if (returnVal == JFileChooser.APPROVE_OPTION)
        {
          File file = fileChooser.getSelectedFile();
          exportImages(file);
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Reset scroll offset.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("resetScroll"))
      {
        setOffset(0, 0);
        currentOffset.setText("CURRENT SCROLLING OFFSET:\nX: "+(coordGrid.gridOffset.x/coordGrid.activeRegionSize.x)+"\nY: "+(coordGrid.gridOffset.y/coordGrid.activeRegionSize.y));
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Toggle whether or not the current Tile has a frame.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("hasFrame"))
      {
        if (mostRecentTile != null)
        {
          println("Changing hasFrame status of tile ID: "+mostRecentTile.id+" to value <"+hasFrame.getValue()+">...");
          if (hasFrame.getValue() < .5f) mostRecentTile.setFrame(false);
          else mostRecentTile.setFrame(true);
        }
        else println("No recent Tile to edit!");
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Toggle whether or not the current Tile is selectable.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("selectable"))
      {
        if (mostRecentTile != null)
        {
          println("Changing selectablity status of tile ID: "+mostRecentTile.id+" to value <"+isSelectable.getValue()+">...");
          if (isSelectable.getValue() < .5f) mostRecentTile.setSelectablity(false);
          else mostRecentTile.setSelectablity(true);
        }
        else println("No recent Tile to edit!");
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Toggle whether or not this tile is the initial tile to have focus when in the actual UI.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("isFirst"))
      {
        if (mostRecentTile != null)
        {
          println("Changing isFirst status of tile ID: "+mostRecentTile.id+" to value <"+isFirst.getValue()+">...");
          if (isFirst.getValue() < .5f)
          {
            mostRecentTile.setFirst(false);
          }
          else
          {
            grid.clearFirstStatus();
            mostRecentTile.setFirst(true);
          }
        }
        else println("No recent Tile to edit!");
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Change Grid Resolution.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("gridRes"))
      {
        println("Changing grid size...");
        coordGrid.gridLines = (int)gridRes.getValue();
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Change current tile's active size.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("activeSize"))
      {
        println("Updating active tile size...");
        if (mostRecentTile != null)
        {
          mostRecentTile.activeSize.x = mostRecentTile.ratio.x * activeSize.getValue();
          mostRecentTile.activeSize.y = mostRecentTile.ratio.y * activeSize.getValue();
        }
        else
        {
          println("No active tile to update, so I'm doing em all.");
          grid.setTileActiveSize(activeSize.getValue(), activeSize.getValue());
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Change current tile's inactive size.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("inactiveSize"))
      {
        println("Updating inactive tile size...");
        if (mostRecentTile != null)
        {
          mostRecentTile.inactiveSize.x = mostRecentTile.ratio.x * inactiveSize.getValue();
          mostRecentTile.inactiveSize.y = mostRecentTile.ratio.y * inactiveSize.getValue();
        }
        else
        {
          println("No active tile to update, so I'm doing em all.");
          grid.setTileInactiveSize(inactiveSize.getValue(), inactiveSize.getValue());
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Update Game Name
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("name"))
      {
        if (mostRecentTile != null)
        {
          println("Updating game name from " + mostRecentTile.name + " to " + name.getText());
          mostRecentTile.name = name.getText();
          updateCurrentTileText();
        }
        else
        {
          println("No active tile to update.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Update Description.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("desc"))
      {
        if (mostRecentTile != null)
        {
          println("Updating game description from "+mostRecentTile.description + " to " + description.getText());
          mostRecentTile.description = description.getText();
          updateCurrentTileText();
        }
        else
        {
          println("No active tile to update.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Update Command.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("command"))
      {
        if (mostRecentTile != null)
        {
          println("Updating game command from " + mostRecentTile.command + " to " + command.getText());
          mostRecentTile.command = command.getText();
          updateCurrentTileText();
        }
        else
        {
          println("No active tile to update.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Update active screenshot path.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("activePath"))
      {
        if (mostRecentTile != null)
        {
          println("Updating active screenshot path from "+mostRecentTile.activePath + " to " + activePath.getText());
          mostRecentTile.initActiveImage(activePath.getText());
          updateCurrentTileText();
        }
        else
        {
          println("No active tile to update.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Updating inactive screenshot path.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("inactivePath"))
      {
        if (mostRecentTile != null)
        {
          println("Updating inactive screenshot path from "+mostRecentTile.inactivePath + " to " + inactivePath.getText());
          mostRecentTile.initInactiveImage(inactivePath.getText());
          updateCurrentTileText();
        }
        else
        {
          println("No active tile to update.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Updating background screenshot path.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("bgPath"))
      {
        if (mostRecentTile != null)
        {
          println("Updating background screenshot path from "+mostRecentTile.bgPath + " to " + bgPath.getText());
          mostRecentTile.initBGImage(bgPath.getText());
          updateCurrentTileText();
        }
        else
        {
          println("No active tile to update.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Active Path Dialog
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("activePathButton"))
      {
        println("Starting file chooser for active path...");
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        int returnVal = fileChooser.showOpenDialog(papplet);

        if (returnVal == JFileChooser.APPROVE_OPTION)
        {
          File file = fileChooser.getSelectedFile();
          activePath.setText(file.getPath());
          activePath.submit();
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Inactive Path Dialog
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("inactivePathButton"))
      {
        println("Starting file chooser for inactive path...");
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        int returnVal = fileChooser.showOpenDialog(papplet);

        if (returnVal == JFileChooser.APPROVE_OPTION)
        {
          File file = fileChooser.getSelectedFile();
          inactivePath.setText(file.getPath());
          inactivePath.submit();
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // BG Path Dialog
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getController().getName().equals("bgPathButton"))
      {
        println("Starting file chooser for bg path...");
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        int returnVal = fileChooser.showOpenDialog(papplet);

        if (returnVal == JFileChooser.APPROVE_OPTION)
        {
          File file = fileChooser.getSelectedFile();
          bgPath.setText(file.getPath());
          bgPath.submit();
        }
      }
    }
    else if (theEvent.isGroup()) {
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Updating north neighbor.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getGroup().getName().equals("north"))
      {        
        if (mostRecentTile != null)
        {
          mostRecentTile.north = grid.getTile(((int)theEvent.getGroup().getValue()));
          println("updated northern neighbor to ID# " + ((int)theEvent.getGroup().getValue()));
        }
        else
        {
          println("No active tile to update.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Updating south neighbor.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getGroup().getName().equals("south"))
      {        
        if (mostRecentTile != null)
        {
          mostRecentTile.south = grid.getTile(((int)theEvent.getGroup().getValue()));
          println("updated southern neighbor to ID# " + ((int)theEvent.getGroup().getValue()));
        }
        else
        {
          println("No active tile to update.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Updating east neighbor.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getGroup().getName().equals("east"))
      {        
        if (mostRecentTile != null)
        {
          mostRecentTile.east = grid.getTile(((int)theEvent.getGroup().getValue()));
          println("updated eastern neighbor to ID# " + ((int)theEvent.getGroup().getValue()));
        }
        else
        {
          println("No active tile to update.");
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Updating west neighbor.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (theEvent.getGroup().getName().equals("west"))
      {        
        if (mostRecentTile != null)
        {
          mostRecentTile.west = grid.getTile(((int)theEvent.getGroup().getValue()));
          println("updated western neighbor to ID# " + ((int)theEvent.getGroup().getValue()));
        }
        else
        {
          println("No active tile to update.");
        }
      }
    }
  }
}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "TileCreator" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
