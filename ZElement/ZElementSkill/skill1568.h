#ifndef __CPPGEN_GNET_SKILL1568
#define __CPPGEN_GNET_SKILL1568
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1568:public Skill
    {
      public:
        enum
        { SKILL_ID = 1568 };
          Skill1568 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1568Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.01 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1568Stub ():SkillStub (1568)
        {
            occupation = 149;
            name = L"¾»ÍÁÕæÑÔII";
            nativename = "¾»ÍÁÕæÑÔII";
            icon = "¾»ÍÁÕæÑÔII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 3;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            action[0] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[1] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[2] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[3] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[4] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[5] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1568Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 548 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 12 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1560 + 150 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 31000 - 1000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1560 + 150 * skill->GetLevel (),
                               skill->GetLevel (), 60, 2, 60, 60, skill->GetLevel (), skill->GetLevel (), 31 - skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (zrand (skill->GetPlayer ()->GetHp () * skill->GetLevel ()) + 30);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (zrand (skill->GetPlayer ()->GetMp () * skill->GetLevel ()) + 30);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
#endif
    };
}
#endif
