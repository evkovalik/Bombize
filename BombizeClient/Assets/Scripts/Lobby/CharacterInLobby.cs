using UnityEngine;

public class CharacterInLobby
{
    private GameObject _characterInLobby;
    private readonly Vector3 _spawnPosition = new(0f, -0.06f, 0f);
    private readonly Vector3 _scale = new(0.6f, 0.6f, 1f);
    private int _currentCharacterId = 0;

    public void CreateOrUpdate(int id)
    {
        if(_characterInLobby != null)
        {
            if(id == _currentCharacterId) return;
            
            _characterInLobby.SetActive(false);
            Object.Destroy(_characterInLobby);
        }
        _characterInLobby = Object.Instantiate(
            Locator.GameStorage.Instance.Characters[id].Prefab,
            _spawnPosition,
            Quaternion.identity
        );
        _characterInLobby.transform.localScale = _scale;
        _currentCharacterId = id;
    }
}