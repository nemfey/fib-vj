using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static System.Collections.Specialized.BitVector32;
//using TMPro;

public class PlayerMovement : MonoBehaviour
{
    // Use this script
    private GameManager gameManagerScript;
    public bool proceduralLevel = true;
    public bool endGameSection = false;

    public Material lightWallMaterial;

    public bool godMode = false;
    private float jumpTime = 0f;

    public GameObject level;
    public GameObject playerModel;

    public Animator animController;
    private AudioManager audioManager;
    private DeathParticleController deathParticles;

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

    public bool bAlive = true;
    public float limitY;

    [SerializeField] private GameObject gameCanvas;

    public Vector3 cameraPosition;
    public float cameraSpeed;

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
        GameObject gameManagerObject = GameObject.Find("GameManager");
        gameManagerScript = gameManagerObject.GetComponent<GameManager>();
        proceduralLevel = true;
        endGameSection = false;
        godMode = false;

        rb = GetComponent<Rigidbody>();

        animController = GetComponentInChildren<Animator>();
        audioManager = FindObjectOfType<AudioManager>();
        deathParticles = GetComponentInChildren<DeathParticleController>();

        bAlive = true;

        cameraPosition = new Vector3(2.5f, 50f, -12.5f);
        cameraSpeed = 8f;
    }

    // Update is called once per frame
    void Update()
    {
        if (Time.timeScale != 0f)
        {
            moveForward();
            moveToCenter();
            rotateToTargetAngle();

            if (godMode)
            {
                jumpTime -= Time.deltaTime;
                godModeMovement();
            }
            else if (Input.GetKeyDown(KeyCode.Space))
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

            checkIfFallDeath();
        }
    }

    void moveForward()
    {
        if (bAlive) transform.position += transform.forward * velocity * Time.deltaTime;
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
            //audioManager.playSound("dJump");
            audioManager.playSound("dJump");

            jumpProcedure();
        }
        else if (Physics.Raycast(transform.position, Vector3.down, out hitInfo))
        {
            string collider_tag = hitInfo.collider.tag;
            if ((collider_tag != "RightTurn" && collider_tag != "LeftTurn") && jumpCount < 2)
            {
                animController.SetBool("InAir", true);
                //audioManager.playSound("jump");
                audioManager.playSound("dJump");

                jumpProcedure();
            }
            else if ((collider_tag == "RightTurn" || collider_tag == "LeftTurn") && bGrounded)
            {
                checkBarrelActivation(hitInfo);
                changeCameraPosition(hitInfo);

                if ((collider_tag == "RightTurn" && targetAngle == 90f) || (collider_tag == "LeftTurn" && targetAngle == 0f))
                {
                    animController.SetBool("InAir", true);
                    //audioManager.playSound("jump");
                    audioManager.playSound("dJump");

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

                    illuminateTurnPlatform(hitInfo);

                    // Add score
                    int score = PlayerPrefs.GetInt("ScoreCount", 0) + 1;
                    PlayerPrefs.SetInt("ScoreCount", score);
                    audioManager.playSound("pointEarn");

                    if (proceduralLevel)
                        level.GetComponent<CreateLevel>().newSectionProcedure();
                }
            }
        }
    }

    void godModeMovement()
    {
        // Check floor below
        if (Physics.Raycast(transform.position, Vector3.down, out hitInfo))
        {
            string collider_tag = hitInfo.collider.tag;
            if (((collider_tag == "RightTurn" && targetAngle == 0f) || (collider_tag == "LeftTurn" && targetAngle == 90f)) && bGrounded)
            {
                checkBarrelActivation(hitInfo);
                changeCameraPosition(hitInfo);

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

                illuminateTurnPlatform(hitInfo);

                // Add score
                int score = PlayerPrefs.GetInt("ScoreCount", 0) + 1;
                PlayerPrefs.SetInt("ScoreCount", score);
                audioManager.playSound("pointEarn");

                if (proceduralLevel)
                    level.GetComponent<CreateLevel>().newSectionProcedure();
            }
        }

        Vector3 raycastOffset = transform.position + new Vector3(0f, 0f, 2f);
        Vector3 raycastMoreOffset = transform.position + new Vector3(0f, 0f, 2.5f);
        if (targetAngle == 90f)
        {
            raycastOffset = transform.position + new Vector3(2f, 0f, 0f);
            raycastMoreOffset = transform.position + new Vector3(2.5f, 0f, 0f);
        }

        if (Physics.Raycast(raycastOffset, Vector3.down, out hitInfo) || Physics.Raycast(raycastMoreOffset, Vector3.down, out hitInfo))
        {
            string collider_tag = hitInfo.collider.tag;
            if (collider_tag == "FallingFloor" && jumpCount < 2 && jumpTime <= 0f)
            {
                if (jumpCount == 0)
                {
                    animController.SetBool("InAir", true);
                    //audioManager.playSound("jump");
                    audioManager.playSound("pickOption");
                }
                else
                {
                    animController.SetTrigger("DJumpTrigger");
                    //audioManager.playSound("dJump");
                    audioManager.playSound("pickOption");
                }
                jumpProcedure();

                jumpTime = 0.5f;
            }
        }
        else if (jumpCount < 2 && jumpTime <= 0f)
        {
            if (jumpCount == 0)
            {
                animController.SetBool("InAir", true);
                //audioManager.playSound("jump");
                audioManager.playSound("pickOption");
            }
            else
            {
                animController.SetTrigger("DJumpTrigger");
                //audioManager.playSound("dJump");
                audioManager.playSound("pickOption");
            }
            jumpProcedure();

            jumpTime= 0.5f;
        }
    }

    void jumpProcedure()
    {
        rb.velocity = Vector3.zero;
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

                GameObject childObject = null;

                for (int i = 0; i < sectionTransform.childCount; i++)
                {
                    Transform child = sectionTransform.GetChild(i);
                    if (child.CompareTag("Barrel"))
                    {
                        child.gameObject.SetActive(true);
                        break;
                    }
                    if (child.CompareTag("FallingFloor"))
                    {
                        Rigidbody rbFallingFloor = child.gameObject.GetComponent<Rigidbody>();
                        rbFallingFloor.useGravity = true;
                    }
                }
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
                sectionObject = parentTransform.GetChild(siblingIndex + 1).gameObject;

                int sectionSize = sectionObject.transform.childCount;
                Vector3 sectionPosition = sectionObject.transform.position;
                Vector3 sectionRotation = sectionObject.transform.eulerAngles;

                // Default
                float offsetXZ = 0f;
                float offsetY = 50f;
                if (sectionSize > 5)
                    offsetXZ = 10f;

                if (sectionObject.name == "EndGameWall(Clone)")
                {
                    endGameSection = true;
                    offsetXZ = 30f;
                    offsetY = 60f;
                    cameraSpeed = 14f;
                }

                cameraPosition = new Vector3(sectionPosition.x - 2.5f, sectionPosition.y + offsetY, sectionPosition.z + offsetXZ + ((sectionSize/2f)*5f));

                if (sectionRotation.y == 90f)
                {
                    cameraPosition = new Vector3(sectionPosition.x + offsetXZ + ((sectionSize/2f)*5f), sectionPosition.y + offsetY, sectionPosition.z - 2.5f);
                }
            }
        }
    }

    void illuminateTurnPlatform(RaycastHit hitInfo)
    {
        GameObject floorObject = hitInfo.collider.gameObject.transform.parent.gameObject;
        GameObject turnPlatform = floorObject.transform.Find("TurnPlatform").gameObject;

        turnPlatform.GetComponent<Renderer>().material = lightWallMaterial;
    }

    void checkIfFallDeath()
    {
        if (transform.position.y < limitY + 25 && bAlive)
        {
            if (endGameSection)
            {
                gameManagerScript.finalCutScene();
                rb.useGravity = false;
            }
            else
            {
                Die("fall");
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

            if (animController != null) animController.SetBool("InAir", false);
        }
        if ((collider_tag == "Obstacle" || collider_tag == "BarrelObstacle") && !godMode)
        {
            Die("generic");
        }
    }

    void OnTriggerEnter(Collider c)
    {
        string collider_tag = c.GetComponent<Collider>().tag;
        if (collider_tag == "Bindweed" && !godMode)
        {
            velocity = velocity / 4;
        }
        if (collider_tag == "CoinObstacle" && !godMode)
        {
            Destroy(c.gameObject);
            Die("generic");
        }
        if (collider_tag == "Obstacle" && !godMode)
        {
            Die("generic");
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
        if (collider_tag == "Bindweed" && !godMode)
        {
            velocity = velocity * 4;
        }
    }

    void Die(string cause)
    {
        bAlive = false;

        if (cause == "generic")
        {
            Destroy(playerModel);

            FindObjectOfType<AudioManager>().stopSound("MainSong");
            FindObjectOfType<AudioManager>().playSound("Death1");
            FindObjectOfType<AudioManager>().playSound("Death2");

            deathParticles.ActivateParticleSystem();
        }
        else if (cause == "fall")
        {
            FindObjectOfType<AudioManager>().stopSound("MainSong");
            FindObjectOfType<AudioManager>().playSound("falling");

            Destroy(playerModel, 3.0f);
        }
    }
}
