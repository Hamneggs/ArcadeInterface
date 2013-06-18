import controlP5.*;
import javax.swing.JFileChooser;
import javax.swing.UIManager;

UI ui;
ControlP5 cp5;

void setup()
{
  
  cp5 = new ControlP5(this);
  
  ui = new UI(cp5);
  
  size(1280, 768);
}

void draw()
{
  background(0, 0, 0);
  ui.render();
  frame.setTitle(String.format("Tile Creator by Gerard Geer"));
}

void keyPressed()
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
