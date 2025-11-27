using UnityEngine;

public class MoveCamera : MonoBehaviour
{
    [SerializeField] private float _smooth = 10f;
    [SerializeField] private float _sideSizeX = 6.5f;
    [SerializeField] private float _sideSizeY = 3.5f;
    private Transform _transformCamera;
    private Transform _transformOther;
    private Vector3 _nextPosition = new(0f, 0f, -10f);
    private Vector2 _limitations;

    public void SetTarget(Transform player, Vector2 limitations)
    {
        _transformOther = player;
        _limitations = limitations;
    }

    private void Start()
    {
        _transformCamera = GetComponent<Transform>();
    }

    private void FixedUpdate()
    {
        _nextPosition.x = Mathf.Clamp(
            _transformOther.position.x, _sideSizeX, _limitations.x - _sideSizeX - 1f
        );
        _nextPosition.y = Mathf.Clamp(
            _transformOther.position.y, _sideSizeY, _limitations.y - _sideSizeY - 1f
        );
        _transformCamera.position = 
            Vector3.Lerp(_transformCamera.position, _nextPosition, Time.fixedDeltaTime * _smooth);
    }

}
