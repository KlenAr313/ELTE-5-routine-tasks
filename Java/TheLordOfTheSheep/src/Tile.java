public interface Tile {
    public default boolean isEmpty(){
        return false;
    }
    public default boolean isAvailableForDog(){
        return false;
    }

    public default boolean isDog(){
        return false;
    }
}
