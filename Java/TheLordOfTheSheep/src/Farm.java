import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Farm {
    public int length;
    public int width;
    private int dogCount;
    private int sheepCount;
    private int waitTime;
    public Tile[][] farm;
    private List<Animal> animals;
    private Boolean escaped;

    public int lengthThird;
    public int widthThird;
    public int minInnerLength;
    public int minInnerWidth;

    public Farm(int length, int width, int dogCount, int sheepCount, int waitTime) {
        Create(length, width, dogCount, sheepCount, waitTime);
    }

    public Farm() {
        Create(14,14,5,10, 500);
    }

    private void Create(int length, int width, int dogCount, int sheepCount, int waitTime) {
        this.length = length;
        this.width = width;
        this.dogCount = dogCount;
        this.sheepCount = sheepCount;
        this.waitTime = waitTime;
        farm = new Tile[length][width];
        animals = new ArrayList<>();
        escaped = false;

        this.lengthThird = (length - 2) / 3;
        this.widthThird = (width - 2) / 3;
        this.minInnerLength = lengthThird + 1;
        this.minInnerWidth = widthThird + 1;

        int shC = 0;
        for (int i = minInnerLength; i < minInnerLength + lengthThird; i++) {
            for (int j = minInnerWidth; j < minInnerWidth + widthThird; j++) {
                if(shC < sheepCount) {
                    farm[i][j] = new Sheep(shC, i, j, waitTime);
                    animals.add((Animal)farm[i][j]);
                    shC++;
                }
                else{
                    farm[i][j] = new Empty(false);
                }
            }
        }

        ArrayList<IntPair> possibleDogCoords = new ArrayList<IntPair>();
        for(int i = 0; i < length; i++){
            for(int j = 0; j < width; j++){
                if(i == 0 || j == 0 || i == length - 1 || j == width - 1){
                    farm[i][j] = new Fence();
                }
                else if(!(i >= minInnerLength && i < minInnerLength + lengthThird && j >= minInnerWidth && j < minInnerWidth + widthThird)){
                    possibleDogCoords.add(new IntPair(i, j));
                }
            }
        }

        Random rand = new Random();
        int index = rand.nextInt(possibleDogCoords.size());
        int x = possibleDogCoords.get(index).first;
        int y = possibleDogCoords.get(index).second;
        for (int i = 0; i < dogCount; i++) {
            farm[x][y] = new Dog(i, x, y, waitTime);
            animals.add((Animal)farm[x][y]);
            possibleDogCoords.remove(index);
            index = rand.nextInt(possibleDogCoords.size());
            x = possibleDogCoords.get(index).first;
            y = possibleDogCoords.get(index).second;
        }

        possibleDogCoords.forEach(pair -> farm[pair.first][pair.second] = new Empty(true));

        farm[rand.nextInt(length - 2) + 1][0] = new Empty(false);
        farm[rand.nextInt(length - 2) + 1][width-1] = new Empty(false);
        farm[0][rand.nextInt(width - 2) + 1] = new Empty(false);
        farm[length-1][rand.nextInt(width - 2) + 1] = new Empty(false);
    }

    public void StartFarm() {
        animals.forEach(animal -> new Thread(() -> animal.Start(this)).start());
        while(!escaped){
            try {
                this.PrintFarm();
                Thread.sleep(waitTime);
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        animals.forEach(animal -> animal.SwitchEscapedStatus());
        System.out.println(escaped);
    }

    public void SheepEscaped(char name){
        System.out.println("Sheep escaped: " + name);
        escaped = true;
    }

    public void PrintFarm()
    {
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                System.out.print(farm[i][j].toString());
            }
            System.out.println();
        }
        System.out.println();
    }
}
