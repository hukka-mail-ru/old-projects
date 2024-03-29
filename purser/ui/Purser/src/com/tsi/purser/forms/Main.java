package com.tsi.purser.forms;

import java.util.Date;
import java.util.TimeZone;

import javax.microedition.lcdui.*;

import com.tsi.purser.Midlet;
import com.tsi.purser.data.*;
import com.tsi.purser.exceptions.InvalidSymbolException;
import com.tsi.purser.exceptions.NoDataException;

public class Main implements Widget, ItemCommandListener, CommandListener 
{
	private Form form = new Form(UserData.Header);
	
	private final String labelName = "Name/ID";
	private final String labelFlight = "Flight";
	private final String labelDate = "Date";
		    
	private TextField fieldName = new TextField(labelName + ":", "", 32, TextField.ANY);
	private TextField fieldFlight = new TextField(labelFlight + ":", "", 32, TextField.ANY); 
	private DateField fieldDate = new DateField("", DateField.DATE, TimeZone.getTimeZone("GMT"));
	private ChoiceGroup fieldPurser = new ChoiceGroup("", Choice.MULTIPLE);
	private StringItem buttonSend = new StringItem("", "Send Request", Item.BUTTON);  
	private StringItem buttonSettings = new StringItem("", "Settings", Item.BUTTON);  
	
	private Command commandExit = new Command("Exit", Command.EXIT, 0);    
	private Command commandSend = new Command("Send", Command.OK, 0);   
	private Command commandSettings = new Command("Settings", Command.OK, 0);   
	
	private Midlet midlet; 
	
	public Main(Midlet m)
	{
		midlet = m;
		
    	fieldPurser.append("Purser", null);
        
        buttonSend.addCommand(commandSend);
        buttonSend.setItemCommandListener(this);   
        buttonSend.setLayout(ImageItem.LAYOUT_CENTER);       
        
        buttonSettings.addCommand(commandSettings);
        buttonSettings.setItemCommandListener(this);   
        buttonSettings.setLayout(ImageItem.LAYOUT_CENTER);  
        
        form.append(fieldName); 
        form.append(fieldDate); 
        form.append(fieldFlight);
        form.append(fieldPurser); 
        form.append(buttonSend);
        form.append(buttonSettings);
                              
        form.addCommand(commandExit);
        form.setCommandListener(this);   
	}
	
	public Item getDefaultItem()
	{
        if(fieldName.getString() != null && fieldName.getString().length() != 0)
    	{        	
        	return fieldDate;
    	}
        else
        {
        	return fieldName;
        }
	}
	
	
    public void getData(UserData userData)
    {
    	userData.name = fieldName.getString().length() == 0 ? 
    			UserData.Nothing : fieldName.getString();
    	
    	userData.flight = fieldFlight.getString().length() == 0  ? 
    			UserData.Nothing : fieldFlight.getString();
    	
    	userData.date = fieldDate.getDate() == null ? 
    			UserData.Nothing : "" + fieldDate.getDate().getTime();
    	
    	boolean[] selected = new boolean[fieldPurser.size()];  
    	fieldPurser.getSelectedFlags(selected);
    	userData.purser = "" + selected[0];    	    	
    }
    

    public void setData(UserData userData)
    {  	
		if(!userData.name.equals(UserData.Nothing))
		{
			fieldName.setString(userData.name);
		}
		
		if(!userData.flight.equals(UserData.Nothing))
		{
			fieldFlight.setString(userData.flight);
		}
		
    	if(!userData.date.equals(UserData.Nothing))
    	{
    		Date d = new Date(Long.parseLong(userData.date));
    		fieldDate.setDate(d);
    	}
    	
    	if(userData.purser.equals("true"))
    	{
    		fieldPurser.setSelectedIndex(0, true);
    	}
    }
    
    
    public void commandAction(Command command, Displayable displayable) 
    {      
        if (displayable == form && command == commandExit) 
        {                                         
        	midlet.exitMIDlet();                                           
        }                                                  
    }     
    
    
    public void commandAction(Command command, Item item) 
    {   
    	try
    	{
	        if (item == buttonSend && command == commandSend) 
            {       
            	if(fieldName.getString() == null || fieldName.getString().length() == 0)
            	{
            		throw new NoDataException(labelName);
            	}

            	if(fieldFlight.getString() == null || fieldFlight.getString().length() == 0)
            	{
            		throw new NoDataException(labelFlight);
            	}

            	if(fieldDate == null ||  
            	   fieldDate.getDate() == null ||
            	   fieldDate.getDate().toString() == null || 
            	   fieldDate.getDate().toString().length() == 0)
            	{
            		throw new NoDataException(labelDate);
            	}

            	boolean[] selected = new boolean[fieldPurser.size()];
            	fieldPurser.getSelectedFlags(selected);
            	            	
            	// get data from controls
            	UserData data = new UserData();
            	getData(data);
            	
            	if(data.name.indexOf("|") >= 0)
            	{
            		throw new InvalidSymbolException(labelName);
            	}
            	if(data.flight.indexOf("|") >= 0)
            	{
            		throw new InvalidSymbolException(labelFlight);
            	}
            	
            	// send SMS
            	String message = data.name + "|" + 
            	                 data.flight + "|" +
            	                 data.date + "|" +
            	                 data.purser;            	
            	
            	SMS.send(midlet.getUserData().callCenter, message);
            	
            	Thread.sleep(1500);
            	midlet.showDone();
            }  
	        else if (item == buttonSettings && command == commandSettings)
            {  
	        	midlet.showSettings();
            }
    	}
    	catch(Exception e)
    	{
    		midlet.showMessage(e);
    	}                                  
    }

	public Displayable getWidget() 
	{
		return form;
	}   
}
