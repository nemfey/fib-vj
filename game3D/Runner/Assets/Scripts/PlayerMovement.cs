using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static System.Collections.Specialized.BitVector32;
using TMPro;

public class PlayerMovement : MonoBehaviour
{
    public GameObject level;

    public Animator animController;
    private AudioManager audioManager;

    public float velocity = 10f;

    [SerializeField] Rigidbody rb;
    public float jumpForce = 15f;
    public float jumpCount = 0;

    float targetAngle = 0f;
    float turnSpeed = 720f;

    float smoothness = 10f;
    float centerSection = -12.5f;

    RaycastHit hitInfo;
    bool bGrounded = true;

    [SerializeField] private GameObject gameCanvas;
    private TextMeshProUGUI scoreText;
    public int score = 0;

    public Vector3 cameraPosition;

    //private TextMeshProUGUI coinsText;

    private bool onSlope()
    {
        if (Physics.Raycast(transform.position, Vector3.down, out hitInfo))
        {
            return hitInfo.normal != Vector3.up;
        }
        return false;
    }

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();

        animController = GetComponentInChildren<Animator>();
        audioManager = FindObjectOfType<AudioManager>();

        scoreText = gameCanvas.transform.Find("Score").GetComponent<TextMeshProUGUI>();

        cameraPosition = new Vector3(2.5f, 50f, -12.5f);
    }

    // Update is called once per frame
    void Update()
    {
        if (Time.timeScale != 0f)
        {
            moveForward();
            moveToCenter();
            rotateToTargetAngle();

            if (Input.GetKeyDown(KeyCode.Space))
            {
                playerAction();
            }

            if (onSlope() && bGrounded)
            {
                //Debug.Log("SLOPEDOWN!");
                //rb.AddForce(Vector3.down * 5f * Time.deltaTime);
                // player should be touchingh the slope all the time
                // instead of floating
            }
        }
    }

    void moveForward()
    {
        transform.position += transform.forward * velocity * Time.deltaTime;
    }

    void moveToCenter()
    {
        // how far are we from the center
        float axisValue = targetAngle == 0f ? transform.position.x : transform.position.z;
        float newAxisValue = Mathf.MoveTowards(axisValue, centerSection, Time.deltaTime * smoothness);

        // Assume we just turned right, hence, center X value
        Vector3 newPosition = new Vector3(newAxisValue, transform.position.y, transform.position.z);
        // if we just turned left, then
        if (targetAngle == 90f)
            newPosition = new Vector3(transform.position.x, transform.position.y, newAxisValue);
        transform.position = newPosition;
    }

    void rotateToTargetAngle()
    {
        Quaternion targetRotation = Quaternion.Euler(0, targetAngle, 0);
        transform.rotation = Quaternion.RotateTowards(transform.rotation, targetRotation, turnSpeed * Time.deltaTime);
    }

    void playerAction()
    {
        if (!bGrounded && jumpCount < 2)
        {
            animController.SetTrigger("DJumpTrigger");
            audioManager.playSound("dJump");

            jumpProcedure();
        }
        else if (Physics.Raycast(transform.position, Vector3.down, out hitInfo))
        {
            string collider_tag = hitInfo.collider.tag;
            if ((collider_tag == "Floor" || collider_tag == "Obstacle") && jumpCount < 2)
            {
                audioManager.playSound("jump");
                animController.SetBool("InAir", true);

                jumpProcedure();
            }
            else if ((collider_tag == "RightTurn" || collider_tag == "LeftTurn") && bGrounded)
            {
                checkBarrelActivation(hitInfo);
                changeCameraPosition(hitInfo);

                if ((collider_tag == "RightTurn" && targetAngle == 90f) || (collider_tag == "LeftTurn" && targetAngle == 0f))
                {
                    audioManager.playSound("jump");
                    animController.SetBool("InAir", true);

                    jumpProcedure();
                }
                else
                {
                    if (collider_tag == "RightTurn")
                    {
                        centerSection = hitInfo.collider.bounds.center.z;
                        targetAngle = 90f;
                    }
                    else
                    {
                        centerSection = hitInfo.collider.bounds.center.x;
                        targetAngle = 0f;
                    }

                    // Add score
                    int score = PlayerPrefs.GetInt("ScoreCount", 0) + 1;
                    PlayerPrefs.SetInt("ScoreCount", score);
                    audioManager.playSound("pointEarn");

                    level.GetComponent<CreateLevel>().newSectionProcedure();
                }
            }
        }
    }

    void jumpProcedure()
    {
        rb.AddForce(Vector3.up * jumpForce, ForceMode.VelocityChange);
        jumpCount++;

        bGrounded = false;
    }

    void checkBarrelActivation(RaycastHit hitInfo)
    {
        GameObject floorObject = hitInfo.collider.gameObject.transform.parent.gameObject;
        if (floorObject != null)
        {
            GameObject sectionObject = floorObject.transform.parent.gameObject;
            if (sectionObject != null)
            {
                // Get next section
                Transform parentTransform = sectionObject.transform.parent;
                int siblingIndex = sectionObject.transform.GetSiblingIndex();
                sectionObject = parentTransform.GetChild(siblingIndex + 1).gameObject;
                Transform sectionTransform = sectionObject.transform;

                GameObject barrelObject = null;

                for (int i = 0; i < sectionTransform.childCount; i++)
                {
                    Transform child = sectionTransform.GetChild(i);
                    if (child.CompareTag("Barrel"))
                    {
                        barrelObject = child.gameObject;
                        break;
                    }
                }

                if (barrelObject != null)
                {
                    barrelObject.SetActive(true);
                }


                //if (barrelObject != null && barrelObject.transform.IsChildOf(sectionObject.transform))
                //{
                //    Debug.Log("BARREL IN THIS SECTION!");
                //    barrelObject.SetActive(true);
                //}
            }
        }
    }

    void changeCameraPosition(RaycastHit hitInfo)
    {
        GameObject floorObject = hitInfo.collider.gameObject.transform.parent.gameObject;
        if (floorObject != null)
        {
            GameObject sectionObject = floorObject.transform.parent.gameObject;
            if (sectionObject != null)
            {
                Transform parentTransform = sectionObject.transform.parent;
                int siblingIndex = sectionObject.transform.GetSiblingIndex();
                //sectionObject = parentTransform.GetChild(siblingIndex + 5).gameObject;
                sectionObject = parentTransform.GetChild(siblingIndex + 1).gameObject;

                int sectionSize = sectionObject.transform.childCount;
                Vector3 sectionPosition = sectionObject.transform.position;
                Vector3 sectionRotation = sectionObject.transform.eulerAngles;

                // Default
                cameraPosition = new Vector3(sectionPosition.x - 2.5f, sectionPosition.y + 50f, sectionPosition.z + ((sectionSize/2f)*5f));

                if (sectionRotation.y == 90f)
                {
                    cameraPosition = new Vector3(sectionPosition.x + ((sectionSize/2f)*5f), sectionPosition.y + 50f, sectionPosition.z - 2.5f);
                }
            }
        }
    }

    void OnCollisionEnter(Collision c)
    {
        string collider_tag = c.collider.tag;
        if (collider_tag == "Floor" || collider_tag == "RightTurn" || collider_tag == "LeftTurn")
        {
            jumpCount = 0;
            bGrounded = true;

            animController.SetBool("InAir", false);
        }
        if (collider_tag == "Obstacle" || collider_tag == "BarrelObstacle")
        {
            Debug.Log("IM DEAD!");
        }
    }

    void OnTriggerEnter(Collider c)
    {
        string collider_tag = c.GetComponent<Collider>().tag;
        if (collider_tag == "Bindweed")
        {
            velocity = velocity / 4;
        }
        if (collider_tag == "CoinObstacle")
        {
            Destroy(c.gameObject);
            Debug.Log("IM DEAD!");
        }
        if (collider_tag == "Obstacle")
        {
            Debug.Log("IM DEAD!");
        }
        if (collider_tag == "Coin")
        {
            Destroy(c.gameObject);
            // Update player coins
            int coins = PlayerPrefs.GetInt("CoinCount", 0);
            coins++;
            PlayerPrefs.SetInt("CoinCount", coins);

            audioManager.playSound("coin");
        }
    }

    void OnTriggerExit(Collider c)
    {
        string collider_tag = c.GetComponent<Collider>().tag;
        if (collider_tag == "Bindweed")
        {
            velocity = velocity * 4;
        }
    }
}
