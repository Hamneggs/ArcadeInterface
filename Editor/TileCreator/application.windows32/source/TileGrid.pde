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
