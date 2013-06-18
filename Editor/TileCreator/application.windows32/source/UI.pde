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
                .setValue(coordGrid.gridLines*1.5);
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
          if (hasFrame.getValue() < .5) mostRecentTile.setFrame(false);
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
          if (isSelectable.getValue() < .5) mostRecentTile.setSelectablity(false);
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
          if (isFirst.getValue() < .5)
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
