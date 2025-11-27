using UnityEngine;

[CreateAssetMenu(fileName = "NewCharacter", menuName = "ScrObj/Character")]
public class Character : ScriptableObject
{
    [field: SerializeField] public GameObject Prefab { get; private set; }
    [field: SerializeField] public GameObject BombPrefab { get; private set; }
    [field: SerializeField] public GameObject FirePrefab { get; private set; }
    [field: SerializeField] public string Name { get; private set; }
    [field: SerializeField] public Sprite Icon { get; private set; }
}