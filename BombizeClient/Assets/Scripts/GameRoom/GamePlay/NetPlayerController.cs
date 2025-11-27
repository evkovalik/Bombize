using UnityEngine;
using DataStructures;
using GameNetwork.Types;
using GameNetwork.GameActions;

[RequireComponent(typeof(CharacterView))]
public class NetPlayerController : MonoBehaviour
{
    private CharacterView _characterView;
    private Rigidbody2D _rigidbody2D;
    private Direction _direction;
    private Vector2 _dxVec2 = Vector2.zero;
    private Vector2 _dyVec2 = Vector2.zero;

    private ActionDispatcher _dispatcher;
    private const float _timeUpdatePosition = 0.1f;

    private Vector2Fixed16Bit _networkPosition = new(
        new(0f, 40f, 0.01f), new(0f, 40f, 0.01f)
    );
    private float _speed = 0f;
    public float Speed { get => _speed;  set => _speed = value; }

    public void Init(ActionDispatcher dispatcher, float speed)
    {
        _dispatcher = dispatcher;
        _speed = speed;
        InvokeRepeating(nameof(SendPosition), 1f, _timeUpdatePosition);
    }

    private void OnDisable()
    {
        CancelInvoke(nameof(SendPosition));
    }

    private void Start()
    {
        _characterView = GetComponent<CharacterView>();
        var collider = gameObject.AddComponent<CircleCollider2D>();
        collider.radius = 0.48f;
        _rigidbody2D = gameObject.AddComponent<Rigidbody2D>();
        _rigidbody2D.gravityScale = 0.0f;
        _rigidbody2D.freezeRotation = true;
    }

    private void Update()
    {
        if (Input.GetKey(KeyCode.D))
        {
            _direction = Direction.Right;
        }
        else if (Input.GetKey(KeyCode.A))
        {
            _direction = Direction.Left;
        }
        else if (Input.GetKey(KeyCode.W))
        {
            _direction = Direction.Up;
        }
        else if (Input.GetKey(KeyCode.S))
        {
            _direction = Direction.Down;
        }
        else
        {
            _direction = Direction.No;
        }

        if(Input.GetKeyDown(KeyCode.Space))
        {
            _dispatcher.TrySetBomb();
        }
    }

    private void FixedUpdate()
    {
        Vector2 nowPos = _rigidbody2D.position;
        Vector2 needPos = new(Mathf.Round(nowPos.x), Mathf.Round(nowPos.y));

        _dxVec2.x = (needPos.x - nowPos.x) * _speed;
        _dyVec2.y = (needPos.y - nowPos.y) * _speed;

        _rigidbody2D.velocity = _direction switch
        {
            Direction.Up => (Vector2)(Vector3.Normalize(Vector2.up + _dxVec2) * _speed),
            Direction.Down => (Vector2)(Vector3.Normalize(Vector2.down + _dxVec2) * _speed),
            Direction.Left => (Vector2)(Vector3.Normalize(Vector2.left + _dyVec2) * _speed),
            Direction.Right => (Vector2)(Vector3.Normalize(Vector2.right + _dyVec2) * _speed),
            _ => Vector2.zero,
        };
        if (_direction != Direction.No) _characterView.SetDirection(_direction);
    }

    private void SendPosition()
    {
        _networkPosition.Value(transform.position.x, transform.position.y);
        var label = new FloatAsUInt(Time.time);
        _dispatcher.UpdatePosition(_networkPosition, label);
    }
}
