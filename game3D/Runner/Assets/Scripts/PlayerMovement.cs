using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static System.Collections.Specialized.BitVector32;

public class PlayerMovement : MonoBehaviour
{
    public GameObject level;

    public bool isGrounded;

    public float speed;
    public float jumpForce;
    public float jumpCount;
    public float rightTurnAngle;
    public float leftTurnAngle;

    // Start is called before the first frame update
    void Start()
    {
        isGrounded = true;
        speed = 5f;
        jumpForce = 5f;
        jumpCount = 0;
        rightTurnAngle = 90f;
        leftTurnAngle = -90f;
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(Vector3.forward * speed * Time.deltaTime);

        if (Input.GetKeyDown(KeyCode.Space))
        {
            RaycastHit hit;
            if (Physics.Raycast(transform.position, Vector3.down, out hit))
            {
                if (hit.collider.tag == "Floor" && jumpCount < 2)
                {
                    GetComponent<Rigidbody>().AddForce(Vector3.up * jumpForce, ForceMode.Impulse);
                    jumpCount++;
                }
                else if (hit.collider.tag == "RightTurn")
                {
                    transform.Rotate(Vector3.up, rightTurnAngle);
                    level.GetComponent<CreateLevel>().newSectionProcedure();
                }
                else if (hit.collider.tag == "LeftTurn")
                {
                    transform.Rotate(Vector3.up, leftTurnAngle);
                    level.GetComponent<CreateLevel>().newSectionProcedure();
                }
            }
        }

        if (isGrounded)
        {
            jumpCount = 0;
        }
    }
}
