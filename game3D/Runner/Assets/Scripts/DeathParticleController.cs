using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DeathParticleController : MonoBehaviour
{
    private ParticleSystem particleSystem;

    public Transform targetObject;

    private void Awake()
    {
        particleSystem = GetComponent<ParticleSystem>();
        //particleSystem.Stop(); // Asegurarse de que el sistema de partículas esté desactivado al principio
    }

    private void Update()
    {
        if (targetObject != null)
        {
            // Calcula la dirección hacia el objeto objetivo
            Vector3 direction = targetObject.position - transform.position;

            // Mueve el objeto seguidor en dirección al objeto objetivo
            transform.Translate(direction.normalized * Time.deltaTime);
        }
    }

    public void ActivateParticleSystem()
    {
        particleSystem.Play();
    }
}
