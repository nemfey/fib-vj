using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DeathParticleController : MonoBehaviour
{
    private ParticleSystem particleSystem;

    private void Awake()
    {
        particleSystem = GetComponent<ParticleSystem>();
        particleSystem.Stop(); // Asegurarse de que el sistema de partículas esté desactivado al principio
    }

    public void ActivateParticleSystem()
    {
        particleSystem.Play();
    }

    public void StopParticleSystem()
    {
        particleSystem.Stop();
    }
}
