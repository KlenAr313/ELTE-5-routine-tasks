public class Empty implements Tile {
    private boolean available;
    public Empty(int x, int y, Farm farm) {
        if(x >= farm.minInnerLength && x < farm.minInnerLength + farm.lengthThird && y >= farm.minInnerWidth && y < farm.minInnerWidth + farm.widthThird) available = false;
        else available = true;
    }

    //In case of door
    public Empty(boolean available) {
        this.available = available;
    }

    @Override
    public boolean isEmpty() {
        return true;
    }

    @Override
    public boolean isAvailableForDog() {
        return available;
    }

    @Override
    public String toString() {
        //return available ? " " : "-";
        return " ";
    }
}
