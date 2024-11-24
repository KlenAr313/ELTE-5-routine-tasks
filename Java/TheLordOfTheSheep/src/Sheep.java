import java.util.ArrayList;
import java.util.Random;

public class Sheep implements Animal {
    public char name;
    private int x;
    private int y;
    private int waitTime;
    private Boolean escaped = false;

    public Sheep(int name, int x, int y, int waitTime) {
        this.name = (char)((name) % 25 + 65);
        this.x = x;
        this.y = y;
        this.escaped = escaped;
        this.waitTime = waitTime;
    }

    public void Start(Farm farm) {
        ArrayList<IntPair> ways = new ArrayList<IntPair>();
        Random rand = new Random();
        boolean moved;
        while(!escaped) {
            moved = false;
            ways.clear();
            try{

                synchronized (farm.farm){
                    //first checking if dogs blocking the way
                    if(farm.farm[x+1][y-1].isDog() || farm.farm[x+1][y].isDog() || farm.farm[x+1][y+1].isDog()) ways.add(new IntPair(x-1, y)); //up dog
                    if(farm.farm[x-1][y+1].isDog() || farm.farm[x][y+1].isDog() || farm.farm[x+1][y+1].isDog()) ways.add(new IntPair(x, y-1)); //right dog
                    if(farm.farm[x-1][y-1].isDog() || farm.farm[x-1][y].isDog() || farm.farm[x-1][y+1].isDog()) ways.add(new IntPair(x-1, y)); //down dog
                    if(farm.farm[x+1][y-1].isDog() || farm.farm[x][y-1].isDog() || farm.farm[x-1][y-1].isDog()) ways.add(new IntPair(x, y+1)); //left dog

                    if(ways.size() > 0) {
                        for (int i = 0; i < ways.size(); i++) {
                            IntPair fs = ways.get(i);
                            if(farm.farm[fs.first][fs.second].isEmpty()){
                                Switch(new IntPair(x,y), fs, farm);
                                moved = true;
                                break;
                            }
                        }
                    }

                    //in case no intended route found or all was blocked
                    if( ways.size() == 0 || !moved){
                        ways.clear();
                        if(farm.farm[x+1][y].isEmpty()) ways.add(new IntPair(x+1, y)); //up
                        if(farm.farm[x][y+1].isEmpty()) ways.add(new IntPair(x, y+1)); //right
                        if(farm.farm[x-1][y].isEmpty()) ways.add(new IntPair(x-1, y)); //down
                        if(farm.farm[x][y-1].isEmpty()) ways.add(new IntPair(x, y-1)); //left
                        if(ways.size() > 0)
                            Switch(new IntPair(x,y), ways.get(rand.nextInt(ways.size())), farm);
                    }

                    if(x == 0 || y == 0 || x == farm.length - 1 || y == farm.width - 1) {
                        farm.SheepEscaped(name);
                        break;
                    }
                }

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
    public String toString() {
        return Character.toString(name);
    }
}
