package systems;

public class SpacecraftData {
    public volatile boolean isLifeOnBoard;

    public SpacecraftData(boolean isLifeOnBoard) {
        this.isLifeOnBoard = isLifeOnBoard;
    }
}
