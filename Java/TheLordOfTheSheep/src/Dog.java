import java.util.ArrayList;
import java.util.Random;

public class Dog implements Animal{
    private int name;
    private int x;
    private int y;
    private int waitTime;
    private Boolean escaped = false;

    public Dog(int name, int x, int y, int waitTime) {
        this.name = name;
        this.x = x;
        this.y = y;
        this.waitTime = waitTime;
    }

    public void Start(Farm farm) {
        ArrayList<IntPair> ways = new ArrayList<IntPair>();
        Random rand = new Random();
        while(!escaped) {
            try{
                synchronized (farm.farm) {
                    if(farm.farm[x+1][y].isAvailableForDog() && farm.farm[x+1][y].isEmpty()) ways.add(new IntPair(x+1,y)); //up
                    if(farm.farm[x][y+1].isAvailableForDog() && farm.farm[x][y+1].isEmpty()) ways.add(new IntPair(x,y+1)); //right
                    if(farm.farm[x-1][y].isAvailableForDog() && farm.farm[x-1][y].isEmpty()) ways.add(new IntPair(x-1,y)); //down
                    if(farm.farm[x][y-1].isAvailableForDog() && farm.farm[x][y-1].isEmpty()) ways.add(new IntPair(x,y-1)); //left

                    //System.out.println(ways.size());
                    if(ways.size() > 0)
                        Switch(new IntPair(x,y), ways.get(rand.nextInt(ways.size())), farm);
                }
                ways.clear();
                Thread.sleep(waitTime);
            }
            catch(InterruptedException e){
                e.printStackTrace();
            }
        }
    }

    private void Switch(IntPair pair, IntPair pair2, Farm farm) {
        farm.farm[pair2.first][pair2.second] = this;
        farm.farm[pair.first][pair.second] = new Empty(pair.first, pair.second, farm);
        this.x = pair2.first;
        this.y = pair2.second;
    }


    public void SwitchEscapedStatus() {
        escaped = true;
    }

    @Override
    public boolean IsDog() {
        return true;
    }

    @Override
    public String toString() {
        return Integer.toString(name);
    }
}
