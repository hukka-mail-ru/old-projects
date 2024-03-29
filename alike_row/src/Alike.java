
import java.io.*;
import java.util.Comparator;
import java.util.Collections;
import java.util.Scanner;
import java.util.ArrayList;
import java.lang.Exception;


class Row
{
    public String name;
    public ArrayList<Integer> numbers;
    public int mismatches = 0;
}


class DescSortMismatches implements Comparator<Row>
{
    public int compare(Row r1, Row r2)
    {
        int res = 0;

        if(r1.mismatches < r2.mismatches)
        {
            res = 1;
        }
        else if(r1.mismatches > r2.mismatches)
        {
            res = -1;
        }
        
        return res;
    }
}




public class Alike 
{    
    private ArrayList<Row> rows = new ArrayList<Row>();
    private String fileName;
       
   private void readFile(String name) throws Exception
   {
       fileName = name;
       Scanner scanner = null;
               
       try {
           File file = new File(fileName);  
           scanner = new Scanner(new FileReader(file));

           while (scanner.hasNextLine()){
               processLine( scanner.nextLine() );
           }
       }
       catch (Exception e)  {
           throw new Exception("Ошибка чтения файла '" + fileName + "'. " + e);
       }
       finally  {
           scanner.close();
       }
   }
   
   private void processLine(String line) throws Exception
   {
       Scanner scanner = new Scanner(line);
       scanner.useDelimiter("=");
       if ( scanner.hasNext() )
       {
           String name = scanner.next().trim();
           String value = scanner.next().trim();

           Row row = new Row();
           row.name = name;
           
           ArrayList<Integer> numbers = new ArrayList<Integer>();
           for(int i=0; i<value.length(); i++)
           {
               Character c = new Character(value.charAt(i));
               String s = c.toString();
               try
               {
                   numbers.add(Integer.parseInt(s));
               }
               catch (NumberFormatException e)  {
                   throw new Exception("Неправильные данные в строке '" + name + "'. " + e);
               }
               
           }
           
           row.numbers = numbers;
                   
           rows.add(row);
       }
       

   }
   
   private void checkLength() throws Exception
   {
       for(int row=0; row<rows.size(); row++)
       {
           for(int i=row+1; i<rows.size(); i++)
           {
               if(rows.get(row).numbers.size() != rows.get(i).numbers.size())
               {
                   throw new Exception("Разная длина: '" + rows.get(row).name + "' и '" + rows.get(i).name +  "'");
               }
           }
       }
   }
   
   private void countMismatches()
   {
       for(int row=0; row<rows.size(); row++)
       {
           Row currentRow = rows.get(row);
           for(int num=0; num<currentRow.numbers.size(); num++)
           {
               for(int compared=row+1; compared<rows.size(); compared++)
               {
                   Row comparedRow = rows.get(compared);
                   
                   if(currentRow.numbers.get(num) != comparedRow.numbers.get(num))
                   {
                       currentRow.mismatches++;
                       comparedRow.mismatches++;
                   }
               }
           }
       }
   }
   

   
   private void output()
   {
       Collections.sort(rows, new DescSortMismatches());  
               
       for(Row row : rows)
       {   
           Utf8.println(row.name + ":\t кол-во отличий: " +  row.mismatches);
       }
   }
   
   
    // constuctor
    public Alike() {}
	
    public static void main(String[] args) 
    {
        
        Alike alike = new Alike();
        try
        {
            alike.readFile(args[0]);
            alike.checkLength();
            alike.countMismatches();
            alike.output();
        }
        catch (Exception e)  {
            Utf8.println(e.getMessage());
            e.printStackTrace();
        }
        
	}
}
