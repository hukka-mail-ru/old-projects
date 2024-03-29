package com.tsi.purser.forms;


import javax.microedition.lcdui.*;

import com.tsi.purser.Midlet;
import com.tsi.purser.data.*;

public class Settings implements Widget, CommandListener 
{
	private Form form = new Form("Settings");
	
    private TextField fieldCallCenter = new TextField("Call Center:", "", 32, TextField.ANY);

    private Command commandOK = new Command("OK", Command.OK, 0); 
    private Command commandExit = new Command("Exit", Command.EXIT, 0);          
    private Midlet midlet; 
    private String callCenter;
    
    public String getCallCenter() { return callCenter; }
    
    public Settings (Midlet m) 
    {
    	midlet = m; 
       
    	form.append(fieldCallCenter); 
    	
    	form.addCommand(commandExit);
    	form.addCommand(commandOK);
    	
        form.setCommandListener(this);       	  
    }
            
    public void commandAction(Command command, Displayable displayable) 
    {      
    	try
    	{
	        if (displayable == form && command == commandExit) 
	        {           	        	
	            midlet.showMain();                                           
	        }                                                  
	        if (displayable == form && command == commandOK) 
            {      
            	if(fieldCallCenter.getString() == null || fieldCallCenter.getString().length() == 0)
            	{
            		//throw new Exception("Please provide call center number");
            		midlet.showMain();
            		return;
            	}
            	
            	midlet.getUserData().callCenter = fieldCallCenter.getString();
	        		
            	midlet.showMain();                                           
            }                                                  
    	}
    	catch(Exception e)
    	{
    		midlet.showMessage(e);
    	}
    }   
    
    public void getData(UserData userData)
    {
    	userData.callCenter = fieldCallCenter.getString().length() == 0 ? 
    			UserData.Nothing : 
    		    fieldCallCenter.getString();
    }

    public void setData(UserData userData)
    {  	
		if(!userData.callCenter.equals(UserData.Nothing))
		{
			fieldCallCenter.setString(userData.callCenter);
		}
    }

	public Displayable getWidget() 
	{		
		return form;
	}
    
	public Item getDefaultItem()
	{
		return fieldCallCenter;
	}
}
