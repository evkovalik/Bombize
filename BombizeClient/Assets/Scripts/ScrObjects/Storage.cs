using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

[CreateAssetMenu(fileName = "Storage", menuName = "ScrObj/GameStorage")]
public class Storage : ScriptableObject
{
    [SerializeField] private List<Character> _characters;
    public IReadOnlyList<Character> Characters { get => _characters; }

    [SerializeField] private List<Tile> _mapBlocks;
    public IReadOnlyList<Tile> MapBlocks { get => _mapBlocks; }

    [SerializeField] private List<Sprite> _mapBackgrounds;
    public IReadOnlyList<Sprite> MapBackgrounds { get => _mapBackgrounds; }
    
    [SerializeField] private List<GameObject> _mapResources;
    public IReadOnlyList<GameObject> MapResources { get => _mapResources; }
}
